#include "Bno.hpp"

Bno::Bno(uint8_t i2c, unsigned long interval_ms)
    : _bno(-1, i2c, &Wire),
      _i2c(i2c),
      _isInit(false),
      _lrt(0),
      _interval_ms(interval_ms),
      _headingOffset(0.0f) {}

bool Bno::init(TwoWire &bus) {
    _bno = Adafruit_BNO055(-1, 0x28, &bus);

    if (_bno.begin(OPERATION_MODE_IMUPLUS)) {
        delay(100);
        _bno.setExtCrystalUse(true);
        _isInit = true;
        return true;
    }

    delay(200);

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

float Bno::getQuatYaw() const {
    double q0 = qData.w();
    double q1 = qData.x();
    double q2 = qData.y();
    double q3 = qData.z();

    double yaw = -atan2(2.0 * (q0 * q3 + q1 * q2), 1.0 - 2.0 * (q2 * q2 + q3 * q3));
    return (float)(yaw * 180.0 / M_PI);
}

imu::Vector<3> Bno::getEuler() const { 
    imu::Vector<3> mdata = eData;
    if(mdata.x() > 180.0f) {
        mdata.x() -= 360.0f;
    }
    return mdata;
}

imu::Quaternion Bno::getQuat() const { return qData; }
imu::Vector<3> Bno::getLinealAcc() const { return aData; }
bool Bno::isUp() const { return _isInit; }

void Bno::resetHeading() {
    _headingOffset = getEuler().x();
}

float Bno::getRelativeYaw() const {
    float currentYaw = getEuler().x();
    float relativeYaw = currentYaw - _headingOffset;

    while (relativeYaw > 180.0f)  relativeYaw -= 360.0f;
    while (relativeYaw < -180.0f) relativeYaw += 360.0f;

    return relativeYaw;
}