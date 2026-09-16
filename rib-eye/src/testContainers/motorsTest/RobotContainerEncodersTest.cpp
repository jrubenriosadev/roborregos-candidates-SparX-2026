#include "RobotContainerEncodersTest.h"

RobotContainerEncodersTest::RobotContainerEncodersTest() 
    : motor(Pins::Motors::kLeftMotorIN1, Pins::Motors::kLeftMotorIN2, Pins::Motors::kLeftEncoderA, Pins::Motors::kLeftEncoderB),
      timer(0), step(0) {}

void RobotContainerEncodersTest::init() {
    motor.init();

    motor.resetEncoder();
    motor.forward();

    timer = millis();
    step = 1;
}

void RobotContainerEncodersTest::update() {
    static unsigned long lastPrintTime = 0;

    const unsigned long currentTime = millis();

    if (currentTime - lastPrintTime >= 250UL) {
        lastPrintTime = currentTime;

        Serial.print("P: ");
        Serial.println(motor.getEncoder());
    }

    if (step == 1 &&
        currentTime - timer >= 3000UL) {

        motor.stop();

        Serial.print("PF: ");
        Serial.println(motor.getEncoder());

        motor.resetEncoder();
        motor.backward();

        timer = currentTime;
        step = 2;
    }

    if (step == 2 && currentTime - timer >= 3000UL) {

        motor.stop();

        Serial.print("PB: ");
        Serial.println(motor.getEncoder());

        Serial.println(" ---- ends :)");

        step = 3;
    }   
}

