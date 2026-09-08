#include "RobotContainer.h"

RobotContainer::RobotContainer()
    : ultrasonic(Pins::Ultrasonic::kEcho, Pins::Ultrasonic::kTrigger){}

void RobotContainer::init() {
    drive.init();
    ultrasonic.init();

    stateTimer = millis();
    drive.forward();
}

void RobotContainer::update() {
    float distance = 0.0f;
    if (ultrasonic.readAsync(distance, 100)) {
        if (distance > 0.0f) {
            Serial.print("Distancia: ");
            Serial.print(distance);
            Serial.println(" cm");
        } else {
            Serial.println("Distancia: Fuera de rango");
        }
    }

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