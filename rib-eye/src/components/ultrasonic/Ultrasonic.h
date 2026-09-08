#ifndef DEF_ULTRASONIC
#define DEF_ULTRASONIC

#include "Arduino.h"

class Ultrasonic {
    public:
        Ultrasonic(uint8_t echoPin, uint8_t triggerPin);
        void init();
        float readDistance();

        bool readAsync(float &outDistnace, unsigned long intervalMs = 100);
    private:
        const uint8_t echoPin;
        const uint8_t triggerPin;
        float lastDistance;
        unsigned long lastReadTime;
};

#endif