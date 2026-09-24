#ifndef DEF_BNO055
#define DEF_BNO055

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_BNO055.h"
#include "utility/imumaths.h"

enum class States {
    UNINIT,
    UP,
    ERR
}

class Bno {
    public:
        Bno(uint8_t i2c = 0x28, int32_t id = -1, unsigned long interval_ms = 20);
        bool init();
        void update();

        imu::Vector<3> getEuler() const;
        imu::Quaternion getQuat() const;
        imu::Vector<3> getLinealAcc() const;

        void getCalibration(uint8_t* sys, uint8_t* gyro, uint8_t* accel, uint8_t* mag);

        States getState() const;
        bool isUp() const;

    private:
        Adafruit_BNO055 _bno;
        uint8_t _i2c;
        int32_t _id;

        States _state;
        unsigned long _lrt;
        unsigned long _interval_ms;

        imu::Vector<3> eData; // eu
        imu::Quaternion qData; //quat
        imu::Vector<3> aData; //accel
}


#endif