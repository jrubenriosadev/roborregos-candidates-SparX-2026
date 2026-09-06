#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <Arduino.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 1;
    };

    struct Motors {
        static constexpr uint8_t kLeftMotorIN1 = 2;
        static constexpr uint8_t kLeftMotorIN2 = 3;

        static constexpr uint8_t kRightMotorIN1 = 4;
        static constexpr uint8_t kRightMotorIN2 = 5;
    };
}

#endif