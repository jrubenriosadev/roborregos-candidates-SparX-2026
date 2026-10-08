#include "ToF.hpp"

ToF::ToF(uint8_t xShutPin, uint8_t i2c) 
    : _lox(),
      _xShutPin(xShutPin),
      _i2c(i2c),
      _wireBus(nullptr),
      _state(ToFStates::UNINIT),
      _currentDistanceMm(1000),
      _lastMT(0),
      _timeout_ms(150) {}

void ToF::powerOff() {
    pinMode(_xShutPin, OUTPUT);
    digitalWrite(_xShutPin, LOW);
    _state = ToFStates::UNINIT;
}

bool ToF::init(TwoWire &bus) {
    _wireBus = &bus;
    pinMode(_xShutPin, OUTPUT);
    
    digitalWrite(_xShutPin, HIGH);
    delay(10);

    if (!_lox.begin(_i2c, false, _wireBus)) {
        _state = ToFStates::ERR;
        return false;
    }

    _lox.startRangeContinuous();
    _state = ToFStates::MEASURING;
    _lastMT = millis();
    return true;
}

void ToF::update() {
    if (_state == ToFStates::ERR || _state == ToFStates::UNINIT) return;

    if (_lox.isRangeComplete()) {
        uint16_t dist = _lox.readRangeResult();
        
        if (dist > 2000) {
            _currentDistanceMm = 2000; 
        } else {
            _currentDistanceMm = dist;
        }
        
        _state = ToFStates::UP;
        _lastMT = millis();
    } else if (millis() - _lastMT > _timeout_ms) {
        _lox.stopRangeContinuous();
        _lox.startRangeContinuous();
        _lastMT = millis();
        _state = ToFStates::MEASURING;
    }
}

uint16_t ToF::getDistance() const {
    return _currentDistanceMm;
}

bool ToF::isUp() const {
    return _state == ToFStates::UP;
}

ToFStates ToF::getState() const {
    return _state;
}