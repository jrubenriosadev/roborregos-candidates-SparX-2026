#ifndef DEF_DRIVE
#define DEF_DRIVE

#include "Arduino.h"
#include "constants/pinsConfig.h"
#include "components/motor/Motor.h"

class Drive {
    public:
        Drive();

        void init();

        void forward(uint32_t delayMillis);
        void backward(uint32_t delayMillis);
        void turnLeft();
        void turnRight();
        void stop();
        void stop(uint32_t delayMillis);

    private:
        Motor leftMotor, rightMotor;
};

#endif