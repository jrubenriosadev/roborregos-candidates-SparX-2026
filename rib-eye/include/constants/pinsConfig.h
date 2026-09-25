#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <Arduino.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 0;
    };

    struct Motors {
        static constexpr uint8_t kLeftMotorIN1 = 12;
        static constexpr uint8_t kLeftMotorIN2 = 13;
        static constexpr uint8_t kLeftEncoderA = 19;
        static constexpr uint8_t kLeftEncoderB = 18;
        static constexpr uint8_t kLeftPWM = 26;
        static constexpr float kLeftPPR = 492.0f;
        static constexpr bool leftInversion = false;

        static constexpr uint8_t kRightMotorIN1 = 33;
        static constexpr uint8_t kRightMotorIN2 = 32;
        static constexpr uint8_t kRightEncoderA = 16;
        static constexpr uint8_t kRightEncoderB = 17;
        static constexpr uint8_t kRightPWM = 25;
        static constexpr float kRightPPR = 495.0f;;
        static constexpr bool rightInversion = false;

        static constexpr float kWheelDiameter = 0.067f;
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