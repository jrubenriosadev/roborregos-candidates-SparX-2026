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

        static constexpr uint8_t kRightEcho = 34;
        static constexpr uint8_t kRightTrigger = 12;
        
        static constexpr float maxRange = 15.0f;
    };

    struct ToF {
        static constexpr uint8_t kLeftXShut = 13;
        static constexpr uint8_t kLeftAddr = 0x28;

        static constexpr uint8_t kRightXShut = 4;
        static constexpr uint8_t kRightAddr = 0x29;
    };

    struct Motors {
        static constexpr uint8_t kLeftMotorIN1 = 26; // 27
        static constexpr uint8_t kLeftMotorIN2 = 27; // 26
        static constexpr uint8_t kLeftEncoderA = 39; // 17
        static constexpr uint8_t kLeftEncoderB = 36; // 16
        static constexpr uint8_t kLeftPWM = 25;
        static constexpr float kLeftPPR = 480.0f;
        static constexpr bool leftInversion = false;

        static constexpr uint8_t kRightMotorIN1 = 2; // 2
        static constexpr uint8_t kRightMotorIN2 = 15; // 15
        static constexpr uint8_t kRightEncoderA = 18; // 19
        static constexpr uint8_t kRightEncoderB = 19; // 18
        static constexpr uint8_t kRightPWM = 33;
        static constexpr float kRightPPR = 480.0f;;
        static constexpr bool rightInversion = true;
        
        static constexpr float kWheelDiameter = 0.067f;
    };
}

#endif