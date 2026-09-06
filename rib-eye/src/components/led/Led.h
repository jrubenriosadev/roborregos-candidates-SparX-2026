#ifndef DEF_LED
#define DEF_LED

#include "Arduino.h"

class Led {
    private:
        const uint8_t pin;
    
    public:
        explicit Led(uint8_t pin);

        void init();
        void on();
        void off();

};

#endif