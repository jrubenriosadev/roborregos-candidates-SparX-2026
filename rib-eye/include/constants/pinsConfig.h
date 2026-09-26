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
        static constexpr uint8_t kLeftEncoderA = 35;
        static constexpr uint8_t kLeftEncoderB = 34;
        static constexpr uint8_t kLeftPWM = 25;
        static constexpr float kLeftPPR = 475.0f;
        static constexpr bool leftInversion = false;

        static constexpr uint8_t kRightMotorIN1 = 32;
        static constexpr uint8_t kRightMotorIN2 = 33;
        static constexpr uint8_t kRightEncoderA = 19;
        static constexpr uint8_t kRightEncoderB = 18;
        static constexpr uint8_t kRightPWM = 26;
        static constexpr float kRightPPR = 475.0f;;
        static constexpr bool rightInversion = true;

        static constexpr float kWheelDiameter = 0.067f;
    };
}

#endif