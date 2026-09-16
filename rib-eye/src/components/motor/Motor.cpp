#include "Motor.h"

Motor* Motor::a_motor = nullptr;

Motor::Motor(uint8_t pinIN1, uint8_t pinIN2, uint8_t pinEncoderA, uint8_t pinEncoderB) 
    : pinIN1(pinIN1), pinIN2(pinIN2), pinEncoderA(pinEncoderA), pinEncoderB(pinEncoderB), e_count(0), isEncoding(false) {}

void Motor::init() {
    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);

    pinMode(pinEncoderA, INPUT_PULLUP);
    pinMode(pinEncoderB, INPUT_PULLUP);
    stop();

    a_motor = this;
    resetEncoder();

    attachInterrupt(digitalPinToInterrupt(pinEncoderA), Motor::encoderISR, RISING);
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

long Motor::getEncoder() const {
    noInterrupts();
    const long count = e_count;
    interrupts();
    return count;
}

void Motor::resetEncoder() {
    noInterrupts();
    e_count = 0;
    isEncoding = false;
    interrupts();
}

bool Motor::isEncoderC() const {
    noInterrupts();
    const bool act = isEncoding;
    interrupts();
    return act;
}

void Motor::handleEncoder() {
    const bool encoderA = digitalRead(pinEncoderA);
    const bool encoderB = digitalRead(pinEncoderB);

    if (encoderA != encoderB) ++e_count;
    else --e_count;
    
    isEncoding = true;
}

void Motor::encoderISR() {
    if (a_motor != nullptr) {
        a_motor->handleEncoder();
    }
}