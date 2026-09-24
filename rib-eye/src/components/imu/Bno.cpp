#include "Bno.hpp"

Bno::Bno(uint8_t i2c, int32_t id, unsigned long interval_ms)
    : _bno(_id, _i2c),
      _i2c(i2c),
      _id(id),
      _state(States::UNINIT),
      _lrt(0),
      _interval_ms(interval_ms) {}

bool Bno::init(TwoWire &bus) {
    if(!_bno.begin(&bus)) {
        _state = States::ERR;
        return false;
    }

    _bno.setExtCrystalUse(true);
    _state = States::UP;
    return true;
}

void Bno::update() {
    if(_state != States::UP) return;
    if(millis()- _lrt >= _interval_ms) {
        _lrt = millis();
        eData = _bno.getVector(Adafruit_BNO055::VECTOR_EULER);
        qData = _bno.getQuat();
        aData = _bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    }
}

Bno::getEuler() const { return eData; }
Bno::getQuat() const { return qData; }
Bno::getLinealAcc() const { return aData; }
Bno::getState() const { return _state; }