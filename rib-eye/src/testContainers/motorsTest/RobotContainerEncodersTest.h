#ifndef DEF_ENCODER_CONTAINER
#define DEF_ENCODER_CONTAINER

#include "components/motor/Motor.h"
#include "constants/pinsConfig.h"

class RobotContainerEncodersTest {
    public:
        RobotContainerEncodersTest();

        void init();
        void update();
    private:
        Motor motor;

        unsigned long timer;
        uint8_t step;
};

#endif