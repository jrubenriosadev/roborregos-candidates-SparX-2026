#ifndef DEF_MPU_CONTAINER
#define DEF_MPU_CONTAINER

#include "Arduino.h"
#include "constants/pinsConfig.h"
#include "components/imu/Mpu.h"

class RobotContainerMpuTest {
    public:
        RobotContainerMpuTest();

        void init();
        void update();
    
    private:
        Mpu mpu;

};

#endif