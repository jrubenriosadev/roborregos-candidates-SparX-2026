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
        static constexpr uint8_t kLeftEncoderA = 19;
        static constexpr uint8_t kLeftEncoderB = 18;
        static constexpr uint8_t kLeftPWM = 0;
        static constexpr float kLeftPPR = 0;
        static constexpr bool leftInversion = false;

        static constexpr uint8_t kRightMotorIN1 = 14;
        static constexpr uint8_t kRightMotorIN2 = 12;
        static constexpr uint8_t kRightEncoderA = 16;
        static constexpr uint8_t kRightEncoderB = 17;
        static constexpr uint8_t kRightPWM = 0;
        static constexpr float kRightPPR = 0;
        static constexpr bool rightInversion = true;

        static constexpr float kWheelDiameter = 0.06f;
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