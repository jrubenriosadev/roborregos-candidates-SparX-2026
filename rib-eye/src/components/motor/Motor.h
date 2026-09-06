#ifndef DEF_MOTOR
#define DEF_MOTOR

#include <Arduino.h>

class Motor {
    private:
        const uint8_t pinIN1;
        const uint8_t pinIN2;
    public:
        Motor(uint8_t pinIN1, uint8_t pinIN2);

        void init();
        void forward();
        void backward();
        void stop();
};

#endif