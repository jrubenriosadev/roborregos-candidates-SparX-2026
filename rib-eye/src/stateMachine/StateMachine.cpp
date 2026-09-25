#include "StateMachine.hpp"
#include <Arduino.h>

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
      _currentState(States::INIT),
      _pst(0) {}

void StateMachine::update() {
    static unsigned long lastPrint = 0;

    switch(_currentState) {
        case States::INIT:
            Serial.print("*");
            _container.getDrive().prepDistance(0.5f);
            _currentState = States::MOVE;
            break;

        case States::MOVE:
            // _container.getDrive().setOpenLoop(200, 200);

            Serial.print("-");
            if (_container.getDrive().moveToDistance()) {
                _pst = millis();
                _currentState = States::IDLE;
            }
            break;

        case States::IDLE:
            Serial.print("+");
            if (millis() - _pst >= 1000) {
                _container.getDrive().prepDistance(0.5f);
                _currentState = States::MOVE;
            }
            break;
    }
}