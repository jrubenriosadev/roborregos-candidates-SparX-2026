#include "ToF.hpp"

ToF::ToF(uint8_t xShutPin, uint8_t i2c) 
    : _lox(),
      _xShutPin(xShutPin),
      _i2c(i2c),
      _wireBus(nullptr),
      _state(ToFStates::UNINIT),
      _currentDistanceMm(1000),
      _lastMT(0),
      _timeout_ms(150),
      _errCount(0),
      _lastStatus(0) {}

void ToF::powerOff() {
    pinMode(_xShutPin, OUTPUT);
    digitalWrite(_xShutPin, LOW);
    _state = ToFStates::UNINIT;
}

bool ToF::init(TwoWire &bus) {
    _wireBus = &bus;
    
    // Reinicio limpio por hardware
    pinMode(_xShutPin, OUTPUT);
    digitalWrite(_xShutPin, LOW);
    delay(10);
    digitalWrite(_xShutPin, HIGH);
    delay(15);

    if (!_lox.begin(_i2c, false, _wireBus)) {
        _lastStatus = (int)_lox.Status;
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
        _lastStatus = (int)_lox.Status;
        if (_lox.Status != VL53L0X_ERROR_NONE) {
            ++_errCount;
            _state = ToFStates::MEASURING;
        } else {
            _currentDistanceMm = (dist > 2000) ? 2000 : dist;
            _state = ToFStates::UP;
        }
        _lastMT = millis();
    } else if (millis() - _lastMT > _timeout_ms) {
        _lox.stopRangeContinuous();
        _lox.startRangeContinuous();
        _lastMT = millis();
        _state = ToFStates::MEASURING;
    }
}

uint16_t ToF::getDistance() const { return _currentDistanceMm; }
bool ToF::isUp() const { return _state == ToFStates::UP; }
ToFStates ToF::getState() const { return _state; }
uint32_t ToF::getErrorCount() const { return _errCount; }
int ToF::getLastStatus() const { return _lastStatus; }

