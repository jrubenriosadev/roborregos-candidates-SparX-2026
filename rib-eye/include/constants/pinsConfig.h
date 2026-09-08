#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <Arduino.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 0;
    };

    struct Motors {
        static constexpr uint8_t kLeftMotorIN1 = 2;
        static constexpr uint8_t kLeftMotorIN2 = 4;

        static constexpr uint8_t kRightMotorIN1 = 18;
        static constexpr uint8_t kRightMotorIN2 = 19;
    };

    struct Ultrasonic {
        static constexpr uint8_t kEcho = 0;
        static constexpr uint8_t kTrigger = 0;
    };
}

#endif