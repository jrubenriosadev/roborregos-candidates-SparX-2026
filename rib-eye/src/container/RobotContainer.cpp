#include "RobotContainer.h"


RobotContainer::RobotContainer(): leftMotor(Pins::Motors::kLeftMotorIN1, Pins::Motors::kLeftMotorIN2),
              rightMotor(Pins::Motors::kRightMotorIN1, Pins::Motors::kRightMotorIN1) {}

void RobotContainer::init() {
    leftMotor.init();
    rightMotor.init();
}

void RobotContainer::moveForward(int delayMs) {
    leftMotor.forward();
    rightMotor.backward();
    delay(delayMs);
}

void RobotContainer::moveBackward(int delayMs) {
    rightMotor.forward();
    leftMotor.backward();
    delay(delayMs);
}

void RobotContainer::turnRight() {
    leftMotor.forward();
    rightMotor.forward();
    delay(150);
}

void RobotContainer::turnLeft() {
    leftMotor.backward();
    rightMotor.backward();
    delay(150);
}

void RobotContainer::stop() {
    leftMotor.stop();
    rightMotor.stop();
}

void RobotContainer::stop(int delayMs) {
    leftMotor.stop();
    rightMotor.stop();
    delay(delayMs);
}
