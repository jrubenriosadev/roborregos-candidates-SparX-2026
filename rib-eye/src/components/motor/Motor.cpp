#include "Motor.hpp"
#include <math.h>

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

Motor::Motor(
    uint8_t pinIN1, uint8_t pinIN2, uint8_t pinPWM, uint8_t pinEncoderA, uint8_t pinEncoderB, 
    float ppr, float wheelDiameterM, bool inverted) 
: _pinIN1(pinIN1), _pinIN2(pinIN2), _pinPWM(pinPWM), _pinEncoderA(pinEncoderA), _pinEncoderB(pinEncoderB),
  _inverted(inverted), _ppr(ppr), _wheelDiameter(wheelDiameterM), 
  e_count(0), last_count(0), last_vel_time(0), current_rpm(0.0f), current_radps(0.0f), _slotIndex(255) {}

Motor::~Motor() {
    stop();
    detachInterrupt(digitalPinToInterrupt(_pinEncoderA));
    if(_slotIndex < C) {
        instances[_slotIndex] = nullptr;
    }
}

void Motor::init() {
    pinMode(_pinIN1, OUTPUT);
    pinMode(_pinIN2, OUTPUT);
    pinMode(_pinPWM, OUTPUT);

    pinMode(_pinEncoderA, INPUT_PULLUP);
    pinMode(_pinEncoderB, INPUT_PULLUP);
    
    stop();
    resetEncoder();

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

    if(_inverted) {
        speed = -speed;
    }

    if(speed > 0) {
        digitalWrite(_pinIN1, HIGH);
        digitalWrite(_pinIN2, LOW);
        analogWrite(_pinPWM, speed);
    } else if(speed < 0) {
        digitalWrite(_pinIN1, LOW);
        digitalWrite(_pinIN2, HIGH);
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
    digitalWrite(_pinIN1, LOW);
    digitalWrite(_pinIN2, LOW);
    analogWrite(_pinPWM, 0);
}

void Motor::brake() {
    digitalWrite(_pinIN1, HIGH);
    digitalWrite(_pinIN2, HIGH);
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
float Motor::getSpeedMPS() const { return current_rpm/60.0f; }

void Motor::handleEncoder() {
    const bool a = digitalRead(_pinEncoderA);
    const bool b = digitalRead(_pinEncoderB);
    
    int ipp = (a != b) ? 1 : -1;
    if(_inverted) ipp = -ipp;
    e_count += ipp;
}
