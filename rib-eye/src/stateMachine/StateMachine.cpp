#include "StateMachine.hpp"
#include <Arduino.h>

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
      _currentState(States::WAIT_GREEN),
      _redDetectedInTile(false) {}

void StateMachine::update() {
    ColorDetected color = _container.getColorDetected();

    switch (_currentState) {
        case States::WAIT_GREEN:
            _container.getDrive().stop();
            if (color == ColorDetected::GREEN) {
                _currentState = States::PREP_TILE;
            }
            break;

        case States::PREP_TILE: {
            float yaw = _container.getBno().getRelativeYaw();
            _container.getDrive().prepMoveTile(TILE_SIZE_METERS, yaw);
            
            _redDetectedInTile = false; 
            _currentState = States::MOVING_TILE;
            break;
        }

        case States::MOVING_TILE: {
            if (color == ColorDetected::RED) {
                _redDetectedInTile = true;
            }

            float yaw = _container.getBno().getRelativeYaw();
            float distL = _container.getLeftDistance();
            float distR = _container.getRightDistance();

            bool tileCompleted = _container.getDrive().moveTile(yaw, distL, distR, 150.0f);

            if (tileCompleted) {
                if (_redDetectedInTile || color == ColorDetected::RED) {
                    _currentState = States::STOPPED;
                } else {
                    _currentState = States::PREP_TILE;
                }
            }
            break;
        }

        case States::STOPPED:
            _container.getDrive().stop();
            if (color == ColorDetected::GREEN) {
                _currentState = States::PREP_TILE;
            }
            break;
    }
}