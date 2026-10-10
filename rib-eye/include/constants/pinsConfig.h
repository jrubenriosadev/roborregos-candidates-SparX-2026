#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <Arduino.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 0;
    };

    struct LedRGB {
        static constexpr uint8_t R_PIN = 0;
        static constexpr uint8_t G_PIN = 0;
        static constexpr uint8_t B_PIN = 0;
    };

    struct Ultrasonic {
        static constexpr uint8_t kLeftEcho = 35;
        static constexpr uint8_t kLeftTrigger = 14;

        static constexpr uint8_t kRightEcho = 35;
        static constexpr uint8_t kRightTrigger = 14;
        
        static constexpr float maxRange = 15.0f;
    }

    struct Motors {
        static constexpr uint8_t kLeftMotorIN1 = 27; // 27
        static constexpr uint8_t kLeftMotorIN2 = 26; // 26
        static constexpr uint8_t kLeftEncoderA = 17; // 17
        static constexpr uint8_t kLeftEncoderB = 16; // 16
        static constexpr uint8_t kLeftPWM = 25;
        static constexpr float kLeftPPR = 480.0f;
        static constexpr bool leftInversion = false;

        static constexpr uint8_t kRightMotorIN1 = 2; // 2
        static constexpr uint8_t kRightMotorIN2 = 15; // 15
        static constexpr uint8_t kRightEncoderA = 19; // 19
        static constexpr uint8_t kRightEncoderB = 18; // 18
        static constexpr uint8_t kRightPWM = 33;
        static constexpr float kRightPPR = 480.0f;;
        static constexpr bool rightInversion = true;

        static constexpr float kWheelDiameter = 0.067f;
    };
}

#endif