#ifndef DEF_ULTRASONIC
#define DEF_ULTRASONIC

#include "Arduino.h"

class Ultrasonic {
    public:
        Ultrasonic(uint8_t echoPin, uint8_t triggerPin, float maxDistanceCm = 50.0f);
        void init();
        float readDistance();
        bool readAsync(float &outDistance, unsigned long intervalMs = 40);

    private:
        const uint8_t echoPin;
        const uint8_t triggerPin;
        float lastDistance;
        unsigned long lastReadTime;
        unsigned long maxTimeoutUs;
        float maxDistanceCm;
};

#endif