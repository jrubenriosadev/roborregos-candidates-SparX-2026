#include "ToF.hpp"

ToF::ToF(uint8_t xShutPin, uint8_t i2c) 
    : _xShutPin(xShutPin),
      _i2c(i2c),
      _wireBus(nullptr),
      _state(States::UNINIT),
      _currentDistanceMm(8190),
      _lastMT(0),
      _timeout_ms(100) {}

bool ToF::init(TwoWire &bus) {
    _wireBus = &bus;
    pinMode(_xShutPin, OUTPUT);

    digitalWrite(_xShutPin, LOW);
    delay(10);
    digitalWrite(_xShutPin, HIGH);
    delay(10);

    if(!_lox.begin(_i2c, false, _wireBus)) {
        _state = States::ERR;
        return false;
    }

    _lox.startRangeContinuous();
    _state = States::INIT_MEASUREMENT;
    return true;
}

void ToF::update() {
    if(_state == States::ERR || _state == States::UNINIT) return;

    if(_lox.isRangeComplete()) {
        _currentDistanceMm = _lox.readRangeResult();
        _state = States::UP;
        _lastMT = millis();
    } else if(millis() - _lastMT > _timeout_ms) {
        _state = States::INIT_MEASUREMENT;
    }
}

uint16_t ToF::getDistance() const {
    return _currentDistanceMm;
}

bool ToF::isUp() const {
    return _state == States::UP;
}

States ToF::getState() const {
    return _state;
}