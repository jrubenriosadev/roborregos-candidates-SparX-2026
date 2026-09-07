#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "Arduino.h"
#include "subsystems/drive/Drive.h"

class RobotContainer {
    public:
        RobotContainer();

        void init();
        void update();
    
    private:
        Drive drive;
};


#endif