#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "../components/imu/Mpu.hpp"
#include "../subsystems/drive/Drive.hpp"

class RobotContainer {
    public:
        RobotContainer();

        void init();

        Drive& getDrive();
        Mpu& getMpu();

    private:
        Drive _drive;
        Mpu _mpu;
};

#endif