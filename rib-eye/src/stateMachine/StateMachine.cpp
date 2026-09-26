#include "StateMachine.hpp"
#include <Arduino.h>

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
      _currentState(States::INIT),
      _pst(0) {}

void StateMachine::update() {
    switch(_currentState) {
        case States::INIT:
            _container.getDrive().prepDistance(0.5f);
            _currentState = States::MOVE;
            break;

        case States::MOVE:
            if (_container.getDrive().moveToDistance()) {
                _pst = millis();
                _currentState = States::IDLE;
            }
            break;

        case States::IDLE:
            if (millis() - _pst >= 1000) {
                _container.getDrive().prepDistance(0.5f);
                _currentState = States::MOVE;
            }
            break;
    }
}