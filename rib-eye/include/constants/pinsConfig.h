#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <Arduino.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 0;
    };

    struct Motors {
        static constexpr uint8_t kLeftMotorIN1 = 4;
        static constexpr uint8_t kLeftMotorIN2 = 2;

        static constexpr uint8_t kRightMotorIN1 = 19;
        static constexpr uint8_t kRightMotorIN2 = 18;
    };

    struct Ultrasonic {
        static constexpr uint8_t kEcho = 26;
        static constexpr uint8_t kTrigger = 25;
    };
}

#endif