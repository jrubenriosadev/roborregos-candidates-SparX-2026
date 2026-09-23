#include "StateMachine.hpp"

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
     _currentState(States::INIT),
     _pst(0),
     _isLeft(false) {}

void StateMachine::update() {
    _container.getMpu().readAsync(_imuData);

    switch(_currentState) {

        case States::INIT:
            _container.getDrive().prepDistance(1.0f);
            _currentState = States::A_MOVE;
            break;
        case States::A_MOVE:
            if(_container.getDrive().moveToDistance()){
                _pst = millis();
                _currentState = States::A_IDLE;
            }
            break;
        case States::A_IDLE:
            if(millis() - _pst >= 1000) {
                float target = _isLeft ? (_imuData.yaw - 90.0f) : (_imuData.yaw + 90.0f);
                _container.getDrive().prepAngle(target);
                _currentState = States::B_TURN;
            }
            break;
        case States::B_TURN:
            if(_container.getDrive().turnToAngle(_imuData.yaw)) {
                _pst = millis();
                _currentState = States::B_IDLE;
            }
            break;
        case States::B_IDLE:
            if(millis() - _pst >= 1000) {
                _container.getDrive().prepDistance(1.0f);
                _currentState = States::A_MOVE;
            }
            break;
    }
}