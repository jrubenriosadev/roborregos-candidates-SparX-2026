#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <stdint.h>

namespace Pins {

    struct Led {
        static constexpr uint8_t LED_PIN = 1;
    };

    struct Motors {
        static constexpr uint8_t L_IN1 = 2;
        static constexpr uint8_t L_IN2 = 3;

        static constexpr uint8_t R_IN1 = 4;
        static constexpr uint8_t R_IN2 = 5;
    };
}

#endif