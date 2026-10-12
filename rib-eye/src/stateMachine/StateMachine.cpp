#include "StateMachine.hpp"
#include "../include/constants/driveConfig.h"
#include <Arduino.h>
#include <math.h>

namespace Cfg = DriveConfig;

static float normalizeYaw(float a) {
    while (a > 180.0f) a -= 360.0f;
    while (a <= -180.0f) a += 360.0f;
    return a;
}

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
      _currentState(States::WAIT_GREEN),
      _redDetectedInTile(false),
      _lockedYaw(0.0f),
      _waitStartTime(0) {}

void StateMachine::startTurn(float deltaDeg) {
    _lockedYaw = normalizeYaw(_lockedYaw + deltaDeg);
    _container.getDrive().prepAngle(_lockedYaw);
    _currentState = States::TURNING;
}

void StateMachine::update() {
    ColorDetected color = _container.getColorDetected();

    switch (_currentState) {
        case States::WAIT_GREEN: {
            _container.getDrive().stop();
            if (color == ColorDetected::GREEN) {
                _lockedYaw = _container.getBno().getRelativeYaw();
                if (!isfinite(_lockedYaw)) break;
                _currentState = States::PREP_TILE;
            }
            break;
        }

        case States::PREP_TILE: {
            _container.getDrive().prepMoveTile(TILE_SIZE_METERS, _lockedYaw);
            _redDetectedInTile = false;
            _currentState = States::MOVING_TILE;
            break;
        }

        case States::MOVING_TILE: {
            if (color == ColorDetected::RED) _redDetectedInTile = true;

            const float yaw = _container.getBno().getRelativeYaw();
            const bool done = _container.getDrive().moveTile(
                yaw,
                _container.getLeftDistance(),
                _container.getRightDistance(),
                Cfg::kBaseSpeed);

            if (done) {
                _container.getDrive().stop();
                if (_redDetectedInTile || color == ColorDetected::RED) {
                    _currentState = States::STOPPED;
                } else {
                    _waitStartTime = millis();
                    _currentState = States::WAIT_TILE;
                }
            }
            break;
        }

        case States::WAIT_TILE: {
            _container.getDrive().stop();
            if (millis() - _waitStartTime >= Cfg::kDecideDelayMs) {
                _currentState = States::DECIDE;
            }
            break;
        }

        case States::DECIDE: {
            _container.getDrive().stop();

            if (!_container.frontWall()) {
                _currentState = States::PREP_TILE;
                break;
            }

            const bool leftOpen = !_container.leftWall();
            const bool rightOpen = !_container.rightWall();

            if (leftOpen && rightOpen) {
                startTurn(Cfg::kPreferRight ? Cfg::kRightTurnDeg : -Cfg::kRightTurnDeg);
            } else if (rightOpen) {
                startTurn(Cfg::kRightTurnDeg);
            } else if (leftOpen) {
                startTurn(-Cfg::kRightTurnDeg);
            } else if (Cfg::kDeadEndUTurn) {
                startTurn(2.0f * Cfg::kRightTurnDeg);
            } else {
                _currentState = States::STOPPED;
            }
            break;
        }

        case States::TURNING: {
            const float yaw = _container.getBno().getRelativeYaw();
            if (_container.getDrive().turnToAngle(yaw)) {
                _container.getDrive().stop();
                _waitStartTime = millis();
                _currentState = States::WAIT_TURN;
            }
            break;
        }

        case States::WAIT_TURN: {
            _container.getDrive().stop();
            if (millis() - _waitStartTime >= Cfg::kTurnSettleMs) {
                _currentState = States::PREP_TILE;
            }
            break;
        }

        case States::STOPPED: {
            _container.getDrive().stop();
            if (color == ColorDetected::GREEN) {
                _lockedYaw = _container.getBno().getRelativeYaw();
                if (isfinite(_lockedYaw)) _currentState = States::PREP_TILE;
            }
            break;
        }
    }
}