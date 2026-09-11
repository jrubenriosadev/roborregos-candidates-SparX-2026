#ifndef DEF_COLOR_CONTAINER
#define DEF_COLOR_CONTAINER

#include "Arduino.h"
#include "constants/pinsConfig.h"
#include "components/color-sensor/TCS34725/ColorSensor.h"

class RobotContainerColorTest {
    public:
        RobotContainerColorTest();

        void init();
        void update();
    
    private:
        ColorSensor tcs;

};

#endif