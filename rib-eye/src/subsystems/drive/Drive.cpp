#include "Drive.h"

Drive::Drive() 
    : leftMotor(Pins::Motors::kLeftMotorIN1, Pins::Motors::kLeftMotorIN2),
      rightMotor(Pins::Motors::kRightMotorIN1, Pins::Motors::kRightMotorIN2) {}

void Drive::init() {
    leftMotor.init();
    rightMotor.init();
}

void Drive::forward() {
    leftMotor.forward();
    rightMotor.backward();
}

void Drive::backward() {
    rightMotor.forward();
    leftMotor.backward();
}

void Drive::turnRight() {
    leftMotor.forward();
    rightMotor.forward();
}

void Drive::turnLeft() {
    leftMotor.backward();
    rightMotor.backward();
}

void Drive::stop() {
    leftMotor.stop();
    rightMotor.stop();
}
