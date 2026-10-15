#include "Motor.hpp"
#include <math.h>
#include "constants/pinsConfig.h"
#include "../shiftreg/ShiftReg.hpp"

Motor* Motor::instances[Motor::C] = {nullptr};

template<uint8_t I>
void Motor::isrWrapper() {
    if(instances[I] != nullptr) {
        instances[I]->handleEncoder();
    }
}

typedef void (*ISRFunc)();
static const ISRFunc isrTable[8] = {
    &Motor::isrWrapper<0>, 
    &Motor::isrWrapper<1>,
    &Motor::isrWrapper<2>,
    &Motor::isrWrapper<3>,
    &Motor::isrWrapper<4>,
    &Motor::isrWrapper<5>,
    &Motor::isrWrapper<6>,
    &Motor::isrWrapper<7>
};

namespace {
    bool isVirtual(uint8_t p) { return p != Pins::kNan && p>=Pins::kVirtualPinBase;}
    bool isReal(uint8_t p) { return p!= Pins::kNan && p<Pins::kVirtualPinBase;}
    void writeDirPin(uint8_t p, bool v) {
        if(isVirtual(p)) ShiftReg::write(p - Pins::kVirtualPinBase, v);
        else if(isReal(p)) digitalWrite(p, v);
    }

    void setDir(uint8_t in1, uint8_t in2, bool a, bool b) {
        writeDirPin(in1, a);
        writeDirPin(in2, b);
        ShiftReg::flush();
    }
}

Motor::Motor(
    uint8_t pinIN1, uint8_t pinIN2, uint8_t pinPWM, uint8_t pinEncoderA, uint8_t pinEncoderB, float ppr, float wheelDiameterM, bool inverted) 
    : _pinIN1(pinIN1), _pinIN2(pinIN2), _pinPWM(pinPWM), _pinEncoderA(pinEncoderA), _pinEncoderB(pinEncoderB), _inverted(inverted), _ppr(ppr), _wheelDiameter(wheelDiameterM), e_count(0), _lastInterruptTime(0), last_count(0), last_vel_time(0), current_rpm(0.0f), current_radps(0.0f), _slotIndex(255) {}

Motor::~Motor() {
    stop();
    if(_pinEncoderA != Pins::kNan) digitalPinToInterrupt(_pinEncoderA);
    if(_slotIndex < C) {
        instances[_slotIndex] = nullptr;
    }
}

void Motor::init() {
    if(isReal(_pinIN1)) pinMode(_pinIN1, OUTPUT);
    if(isReal(_pinIN2)) pinMode(_pinIN2, OUTPUT);
    pinMode(_pinPWM, OUTPUT);
    
    stop();
    resetEncoder();

    if(_pinEncoderA == Pins::kNan) return;

    pinMode(_pinEncoderA, INPUT_PULLUP);
    pinMode(_pinEncoderB, INPUT_PULLUP);

    for(uint8_t i = 0; i < C; ++i) {
        if(instances[i] == nullptr) {
            _slotIndex = i;
            instances[i] = this;
            attachInterrupt(digitalPinToInterrupt(_pinEncoderA), isrTable[i], RISING);
            break;
        }
    }
}

void Motor::move(int speed) {
    speed = constrain(speed, -255, 255);

    if(_inverted) speed = -speed;

    if(speed > 0) {
        setDir(_pinIN1, _pinIN2, true, false);
        analogWrite(_pinPWM, speed);
    } else if(speed < 0) {
        setDir(_pinIN1, _pinIN2, false, true);
        analogWrite(_pinPWM, -speed);
    } else {
        stop();
    }
}

void Motor::forward(uint8_t speed) {
    move((int)speed);
}

void Motor::backward(uint8_t speed) {
    move(-(int)speed);
}

void Motor::stop() {
    setDir(_pinIN1, _pinIN2, false, false);
    analogWrite(_pinPWM, 0);
}

void Motor::brake() {
    setDir(_pinIN1, _pinIN2, true, true);
    analogWrite(_pinPWM, 255);
}

void Motor::setPPR(float ppr) {
    if(ppr > 0) _ppr = ppr;
} 

void Motor::setWheelDiameter(float diameterM) {
    if(diameterM > 0) _wheelDiameter = diameterM;
} 

void Motor::setInverted(bool i) {
    _inverted = i;
}

float Motor::getPPR() const { return _ppr; }
float Motor::getWheelDiameter() const { return _wheelDiameter; }
bool Motor::isInverted() const { return _inverted; }

long Motor::getEncoder() const {
    noInterrupts();
    const long c = e_count;
    interrupts();
    return c;
}

void Motor::resetEncoder() {
    noInterrupts();
    e_count = 0;
    last_count = 0;
    interrupts();
    last_vel_time = micros();
}

float Motor::getRevolutions() const  {
    return (float)getEncoder() / _ppr;
}

float Motor::getRadians() const {
    return getRevolutions() * 2.0f * M_PI;
}

float Motor::getDistanceMeters() const {
    return getRevolutions() * M_PI * _wheelDiameter;
}

float Motor::updateVelocity() {
    unsigned long currentTime = micros();
    unsigned long dt = currentTime - last_vel_time;

    if(dt < 1000) return current_radps;

    long currentE = getEncoder();
    long delta = currentE - last_count;

    last_count = currentE;
    last_vel_time = currentTime;

    float dtSeconds = (float)dt / 1000000.0f;
    float deltaRev = (float)delta / _ppr;

    current_radps = (deltaRev * 2.0f * M_PI) / dtSeconds;
    current_rpm = (deltaRev / dtSeconds) * 60.0f;

    return current_radps;
}

float Motor::getRPM() const { return current_rpm; }
float Motor::getRadSec() const { return current_radps; }
float Motor::getSpeedMPS() const { return current_radps * (_wheelDiameter * 0.5f); }

void Motor::handleEncoder() {
    unsigned long now = micros();
    
    if (now - _lastInterruptTime < 30) { 
        return;
    }
    _lastInterruptTime = now;

    const bool a = digitalRead(_pinEncoderA);
    const bool b = digitalRead(_pinEncoderB);
    
    int ipp = (a != b) ? 1 : -1;
    if(_inverted) ipp = -ipp;
    e_count += ipp;
}