#include "StateMachine.hpp"
#include <Arduino.h>

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
      _currentState(States::WAIT_R),
      _pst(0),
      _targetAngle(0.0f),
      _lastColor(ColorDetected::NONE) {}

void StateMachine::update() {
    ColorDetected c = _container.getColorDetected();

    switch (_currentState) {
        case States::WAIT_R:
            if(c == ColorDetected::RED) {
                _pst = millis();
                _currentState = States::DELAY_BS;
            }
            break;
        case States::DELAY_BS:
            if(millis() - _pst >= 3000) {
                _lastColor = ColorDetected::RED;
                _container.getDrive().setOpenLoop(100,100);
                _currentState = States::MOVE;
            }
            break;
    }
}