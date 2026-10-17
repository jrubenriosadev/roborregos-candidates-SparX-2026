#include "StateMachine.hpp"
#include "../include/constants/driveConfig.h"
#include <Arduino.h>
#include <math.h>

namespace Cfg = DriveConfig;

static float normalizeY(float a) {
    while (a > 180.0f) a -= 360.0f;
    while (a <= -180.0f) a += 360.0f;
    return a;
}

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
      _currentState(States::WAITING),
      _redDetectedInTile(false),
      _lockedYaw(0.0f),
      _waitStartTime(0) {}

void StateMachine::startTurn(float deltaDeg) {
    _lockedYaw = normalizeY(_lockedYaw + deltaDeg);

    // Solo se acerca si hay una pared frontal dentro de la casilla; si no, gira directo
    if (_container.tofReady() && _container.getTofLeftMm() <= Cfg::kApproachMaxMm) {
        _container.getDrive().prepApproach();
        _currentState = States::APPROACH;
    } else {
        _container.getDrive().prepAngle(_lockedYaw);
        _currentState = States::STEERING;
    }
}

void StateMachine::update() {
    ColorDetected color = _container.getColorDetected();

    switch (_currentState) {
        case States::WAITING: {
            _container.getDrive().stop();
            if(!_container.tofReady()) break;
            if (color == ColorDetected::YELLOW) {
                _lockedYaw = _container.getBno().getRelativeYaw();
                if (!isfinite(_lockedYaw)) break;
                _currentState = States::PREP;
            }
            break;
        }

        case States::PREP: {
            _container.getDrive().prepMoveTile(TILE_SIZE_METERS, _lockedYaw);
            _redDetectedInTile = false;
            _currentState = States::MOVING;
            break;
        }

        case States::MOVING: {
            if (color == ColorDetected::CYAN) _redDetectedInTile = true;

            const float yaw = _container.getBno().getRelativeYaw();
            const bool done = _container.getDrive().moveTile(yaw, _container.getLeftDistance(), _container.getRightDistance(),
                Cfg::kBSpeed);

            if (done) {
                _container.getDrive().stop();
                if (_redDetectedInTile || color == ColorDetected::CYAN) {
                    _currentState = States::STOPPED;
                } else {
                    _waitStartTime = millis();
                    _currentState = States::DELAY;
                }
            }
            break;
        }

        case States::DELAY: {
            _container.getDrive().stop();
            if (millis() - _waitStartTime >= Cfg::kDecideDelayMs) {
                _currentState = States::DECIDE;
            }
            break;
        }

        case States::DECIDE: {
            _container.getDrive().stop();

            if (!_container.frontWall()) {
                _currentState = States::PREP;
                break;
            }

            const bool leftOpen = !_container.leftWall();
            const bool rightOpen = !_container.rightWall();

            if (leftOpen && rightOpen) {
                startTurn(Cfg::kRightPref ? Cfg::kSteerDeg90 : -Cfg::kSteerDeg90);
            } else if (rightOpen) {
                startTurn(Cfg::kSteerDeg90);
            } else if (leftOpen) {
                startTurn(-Cfg::kSteerDeg90);
            } else if (Cfg::kDeadEnd180) {
                startTurn(2.0f * Cfg::kSteerDeg90);
            } else {
                _currentState = States::STOPPED;
            }
            break;
        }

        case States::APPROACH: {
            if (_container.getDrive().approachFront(_container.getTofLeftMm())) {
                _container.getDrive().stop();
                _container.getDrive().prepAngle(_lockedYaw);
                _currentState = States::STEERING;
            }
            break;
        }

        case States::STEERING: {
            const float yaw = _container.getBno().getRelativeYaw();
            if (_container.getDrive().turnToAngle(yaw)) {
                _container.getDrive().stop();
                _waitStartTime = millis();
                _currentState = States::TURN_DELAY;
            }
            break;
        }

        case States::TURN_DELAY: {
            _container.getDrive().stop();
            if (millis() - _waitStartTime >= Cfg::kSteerSms) {
                _currentState = States::PREP;
            }
            break;
        }

        case States::STOPPED: {
            _container.getDrive().stop();
            if (color == ColorDetected::GREEN) {
                _lockedYaw = _container.getBno().getRelativeYaw();
                if (isfinite(_lockedYaw)) _currentState = States::PREP;
            }
            break;
        }
    }
}