#ifndef DEF_TOF
#define DEF_TOF

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_VL53L0X.h"

enum class ToFStates {
    UNINIT,
    MEASURING,
    UP,
    ERR
};

class ToF {
    public:
        ToF(uint8_t xShutPin, uint8_t i2c);
        void powerOff();
        bool init(TwoWire &bus = Wire);
        void update();

        uint16_t getDistance() const;
        bool isUp() const;
        ToFStates getState() const;

    private:
        Adafruit_VL53L0X _lox;
        uint8_t _xShutPin;
        uint8_t _i2c;
        TwoWire* _wireBus;

        ToFStates _state;
        uint16_t _currentDistanceMm;
        unsigned long _lastMT;
        unsigned long _timeout_ms;
};

#endif