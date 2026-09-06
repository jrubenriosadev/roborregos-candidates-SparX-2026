#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "constants/pinsConfig.h"
#include "components/motor/Motor.h"
#include "Arduino.h"

class RobotContainer {
    public:
        Motor leftMotor;
        Motor rightMotor;

        RobotContainer();

        void init();
        void moveForward(int delayMs);
        void moveBackward(int delayMs);
        void turnRight();
        void turnLeft();
        void stop();
        void stop(int delayMs);
};


#endif