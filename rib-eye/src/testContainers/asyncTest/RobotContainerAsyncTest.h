#ifndef DEF_ASYNC_CONTAINER
#define DEF_ASYNC_CONTAINER

#include "Arduino.h"
#include "subsystems/drive/Drive.h"
#include "components/ultrasonic/Ultrasonic.h"
#include "constants/pinsConfig.h"

class RobotContainerAsyncTest {
    public:
        RobotContainerAsyncTest();

        void init();
        void update();
    
    private:
        Ultrasonic ultrasonic_1, ultrasonic_2;
        Drive drive;

        float dist1 = -1.0f;
        float dist2 = -1.0f;

};


#endif