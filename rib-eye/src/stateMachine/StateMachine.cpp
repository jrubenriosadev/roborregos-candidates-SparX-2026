#include "StateMachine.hpp"
#include <Arduino.h>

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
      _currentState(States::INIT),
      _pst(0),
      _targetAngle(0.0f),
      _sideCount(0) {}

void StateMachine::update() {
    switch(_currentState) {
        case States::INIT:
            if (_container.getBno().isUp()) {
                _targetAngle = _container.getBno().getEuler().x();
                _sideCount = 0;
                
                _container.getDrive().prepDistance(SIDE_DISTANCE);
                _currentState = States::MOVE;
            }
            break;

        case States::MOVE:
            if (_container.getDrive().moveToDistance()) {
                _container.getDrive().stop();
                _pst = millis();
                _currentState = States::PBT;
            }
            break;

        case States::PBT:
            if (millis() - _pst >= 300) {
                _targetAngle += 90.0f; 
                if (_targetAngle >= 360.0f) _targetAngle -= 360.0f;

                _container.getDrive().prepAngle(_targetAngle);
                _currentState = States::TURN;
            }
            break;

        case States::TURN:
            if (_container.getBno().isUp()) {
                float currentYaw = _container.getBno().getEuler().x();
                if (_container.getDrive().turnToAngle(currentYaw)) {
                    _container.getDrive().stop();
                    _pst = millis();
                    _sideCount++;
                    _currentState = States::IDLE;
                }
            } else {
                _container.getDrive().stop();
            }
            break;

        case States::IDLE:
            if (millis() - _pst >= 1000) {
                if (_sideCount < 4) {
                    _container.getDrive().prepDistance(SIDE_DISTANCE);
                    _currentState = States::MOVE;
                } else {
                    _currentState = States::END;
                }
            }
            break;

        case States::END:
            _container.getDrive().stop();
            break;
    }
}