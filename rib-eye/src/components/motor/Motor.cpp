#include "Motor.h"

Motor::Motor(uint8_t pinIN1, uint8_t pinIN2) : pinIN1(pinIN1), pinIN2(pinIN2){}

void Motor::init() {
    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);
    stop();
}

void Motor::forward() {
    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinIN2, LOW);
}

void Motor::backward() {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
}

void Motor::stop() {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, LOW);
}