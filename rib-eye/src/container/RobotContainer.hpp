#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "../components/imu/Bno.hpp"
#include "../subsystems/drive/Drive.hpp"

class RobotContainer {
    public:
        RobotContainer();

        void init();
        void update();

        Drive& getDrive();
        Bno& getBno();

    private:
        Drive _drive;
        Bno _bno;
};

#endif