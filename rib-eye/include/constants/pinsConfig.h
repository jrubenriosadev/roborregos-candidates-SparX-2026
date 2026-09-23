#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <Arduino.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 0;
    };

    struct Motors {
        static constexpr uint8_t kLeftMotorIN1 = 32;
        static constexpr uint8_t kLeftMotorIN2 = 33;
        static constexpr uint8_t kLeftEncoderA = 34;
        static constexpr uint8_t kLeftEncoderB = 35;
        static constexpr uint8_t kLeftPWM = 25;
        static constexpr float kLeftPPR = 475.0f;
        static constexpr bool leftInversion = false;

        static constexpr uint8_t kRightMotorIN1 = 12;
        static constexpr uint8_t kRightMotorIN2 = 13;
        static constexpr uint8_t kRightEncoderA = 2;
        static constexpr uint8_t kRightEncoderB = 4;
        static constexpr uint8_t kRightPWM = 26;
        static constexpr float kRightPPR = 475.0f;;
        static constexpr bool rightInversion = true;

        static constexpr float kWheelDiameter = 0.065f;
    };

    struct Ultrasonic {
        static constexpr uint8_t kEcho1 = 0;
        static constexpr uint8_t kTrigger1 = 0;

        static constexpr uint8_t kEcho2 = 0;
        static constexpr uint8_t kTrigger2 = 0;
    };

    struct Mpu {
        static constexpr uint8_t kSCL = 22;
        static constexpr uint8_t kSDA = 21;
    };
}

#endif