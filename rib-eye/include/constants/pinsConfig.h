#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <Arduino.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 0;
    };

    struct Motors {
        static constexpr uint8_t kLeftMotorIN1 = 26;
        static constexpr uint8_t kLeftMotorIN2 = 27;
        static constexpr uint8_t kLeftEncoderA = 19;
        static constexpr uint8_t kLeftEncoderB = 18;
        static constexpr uint8_t kLeftPWM = 25;
        static constexpr float kLeftPPR = 490.0f;
        static constexpr bool leftInversion = false;

        static constexpr uint8_t kRightMotorIN1 = 33;
        static constexpr uint8_t kRightMotorIN2 = 14;
        static constexpr uint8_t kRightEncoderA = 16;
        static constexpr uint8_t kRightEncoderB = 17;
        static constexpr uint8_t kRightPWM = 32;
        static constexpr float kRightPPR = 490.0f;;
        static constexpr bool rightInversion = false;

        static constexpr float kWheelDiameter = 0.067f;
    };
}

#endif