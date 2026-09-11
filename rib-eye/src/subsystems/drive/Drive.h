#ifndef DEF_DRIVE
#define DEF_DRIVE

#include "Arduino.h"
#include "constants/pinsConfig.h"
#include "components/motor/Motor.h"

class Drive {
    public:
        Drive();

        void init();

        void forward();
        void backward();
        void turnLeft();
        void turnRight();
        void stop();
        void enableLeft(bool e);
        void enableRight(bool e);

    private:
        Motor leftMotor, rightMotor;
};

#endif