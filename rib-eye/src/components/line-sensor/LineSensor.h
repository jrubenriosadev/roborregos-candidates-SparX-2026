#ifndef DEF_LINE_SENSOR
#define DEF_LINE_SENSOR

#include "Arduino.h"
#include "constants/levelsConfig.h"

// TCRT5000 apuntando al piso. isWhite() = hay cinta blanca debajo (piso verde = false)
class LineSensor {
    public:
        explicit LineSensor(uint8_t pin);

        void init();
        void update();

        bool isWhite() const;
        uint16_t getRaw() const;

    private:
        uint8_t _pin;
        uint16_t _raw;
        bool _white;
        uint8_t _count;
};

#endif
