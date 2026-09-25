#include "Bno.hpp"

Bno::Bno(uint8_t i2c, unsigned long interval_ms)
    : _bno(-1, i2c, &Wire),
      _i2c(i2c),
      _isInit(false),
      _lrt(0),
      _interval_ms(interval_ms) {}

bool Bno::init(TwoWire &bus) {
    if (_bno.begin(OPERATION_MODE_NDOF)) {
        delay(100);
        _isInit = true;
        return true;
    }

    delay(200);

    uint8_t alt_i2c = (_i2c == 0x28) ? 0x29 : 0x28;
    _bno = Adafruit_BNO055(-1, alt_i2c, &bus);
    
    if (_bno.begin(OPERATION_MODE_NDOF)) {
        delay(100);
        _i2c = alt_i2c;
        _isInit = true;
        return true;
    }

    _isInit = false;
    return false;
}

void Bno::update() {
    if(!_isInit) return;
    if(millis() - _lrt >= _interval_ms) {
        _lrt = millis();
        eData = _bno.getVector(Adafruit_BNO055::VECTOR_EULER);
        qData = _bno.getQuat();
        aData = _bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    }
}

void Bno::getCalibration(uint8_t* sys, uint8_t* gyro, uint8_t* accel, uint8_t* mag) {
    if(_isInit) {
        _bno.getCalibration(sys, gyro, accel, mag);
    }   
}

imu::Vector<3> Bno::getEuler() const { return eData; }
imu::Quaternion Bno::getQuat() const { return qData; }
imu::Vector<3> Bno::getLinealAcc() const { return aData; }
bool Bno::isUp() const { return _isInit; }