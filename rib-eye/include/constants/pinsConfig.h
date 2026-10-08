#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <Arduino.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 0;
    };

    struct LedRGB {
        static constexpr uint8_t R_PIN = 12;
        static constexpr uint8_t G_PIN = 13;
        static constexpr uint8_t B_PIN = 14;
    };

    struct Motors {
        static constexpr uint8_t kStandBy = 13;

        static constexpr uint8_t kLeftMotorIN1 = 26; // 27
        static constexpr uint8_t kLeftMotorIN2 = 27; // 26
        static constexpr uint8_t kLeftEncoderA = 16; // 17
        static constexpr uint8_t kLeftEncoderB = 17; // 16
        static constexpr uint8_t kLeftPWM = 25;
        static constexpr float kLeftPPR = 480.0f;
        static constexpr bool leftInversion = false;

        static constexpr uint8_t kRightMotorIN1 = 2; // 2
        static constexpr uint8_t kRightMotorIN2 = 15; // 15
        static constexpr uint8_t kRightEncoderA = 19; // 
        static constexpr uint8_t kRightEncoderB = 18; // 
        static constexpr uint8_t kRightPWM = 33; // 
        static constexpr float kRightPPR = 480.0f;;
        static constexpr bool rightInversion = true;

        static constexpr float kWheelDiameter = 0.067f;
    };
}

#endif