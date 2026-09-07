#include "Drive.h"

Drive::Drive() 
    : leftMotor(Pins::Motors::kLeftMotorIN1, Pins::Motors::kLeftMotorIN2),
      rightMotor(Pins::Motors::kRightMotorIN1, Pins::Motors::kRightMotorIN2) {}

void Drive::init() {
    leftMotor.init();
    rightMotor.init();
}

void Drive::forward(uint32_t delayMs) {
    leftMotor.forward();
    rightMotor.backward();
    delay(delayMs);
}

void Drive::backward(uint32_t delayMs) {
    rightMotor.forward();
    leftMotor.backward();
    delay(delayMs);
}

void Drive::turnRight() {
    leftMotor.forward();
    rightMotor.forward();
    delay(150);
}

void Drive::turnLeft() {
    leftMotor.backward();
    rightMotor.backward();
    delay(150);
}

void Drive::stop() {
    leftMotor.stop();
    rightMotor.stop();
}

void Drive::stop(uint32_t delayMs) {
    leftMotor.stop();
    rightMotor.stop();
    delay(delayMs);
}