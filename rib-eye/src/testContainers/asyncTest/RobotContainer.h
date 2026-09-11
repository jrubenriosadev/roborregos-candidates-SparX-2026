#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "Arduino.h"
#include "subsystems/drive/Drive.h"
#include "components/ultrasonic/Ultrasonic.h"
#include "constants/pinsConfig.h"

class RobotContainer {
    public:
        RobotContainer();

        void init();
        void update();
    
    private:
        Ultrasonic ultrasonic_1, ultrasonic_2;
        Drive drive;

        float dist1 = -1.0f;
        float dist2 = -1.0f;

};


#endif