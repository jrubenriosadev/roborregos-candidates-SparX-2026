#ifndef DEF_TOF
#define DEF_TOF

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_VL53L0X.h"

enum class States {
    UNINIT,
    INIT_MEASUREMENT,
    AWAIT_DATA,
    UP,
    ERR
};

class ToF {
    public:
        ToF(uint8_t xShutPin, uint8_t i2c);
        bool init(TwoWire &bus = Wire);
        void update();

        uint16_t getDistance() const;
        bool isUp() const;
        States getState() const;

    private:
        Adafruit_VL53L0X _lox;
        uint8_t _xShutPin;
        uint8_t _i2c;
        TwoWire* _wireBus;

        States _state;
        uint16_t _currentDistanceMm;
        unsigned long _lastMT;
        unsigned long _timeout_ms;
};

#endif