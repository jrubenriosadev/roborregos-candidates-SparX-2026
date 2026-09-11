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
        static constexpr uint8_t kEcho1 = 26;
        static constexpr uint8_t kTrigger1 = 25;

        static constexpr uint8_t kEcho2 = 33;
        static constexpr uint8_t kTrigger2 = 27;
    };

    struct Mpu {
        static constexpr uint8_t kSCL = 22;
        static constexpr uint8_t kSDA = 21;
    };
}

#endif