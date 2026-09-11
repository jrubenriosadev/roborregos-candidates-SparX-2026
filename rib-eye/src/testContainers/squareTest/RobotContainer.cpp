#include "RobotContainer.h"

RobotContainer::RobotContainer() {}

void RobotContainer::init() {
    drive.init();

    stateTimer = millis();
    drive.forward();
}

void RobotContainer::update() {
    unsigned long elapsedTime = millis() - stateTimer;

    switch (currentState) {
        case DriveState::FORWARD:
            if (elapsedTime >= FORWARD_TIME_MS) {
                drive.stop();
                currentState = DriveState::STOP_1;
                stateTimer = millis();
            }
            break;

        case DriveState::STOP_1:
            if (elapsedTime >= STOP_TIME_MS) {
                drive.turnRight();
                currentState = DriveState::TURN_RIGHT;
                stateTimer = millis();
            }
            break;

        case DriveState::TURN_RIGHT:
            if (elapsedTime >= TURN_TIME_MS) {
                drive.stop();
                currentState = DriveState::STOP_2;
                stateTimer = millis();
            }
            break;

        case DriveState::STOP_2:
            if (elapsedTime >= STOP_TIME_MS) {
                drive.forward();
                currentState = DriveState::FORWARD;
                stateTimer = millis();
            }
            break;
    }
}