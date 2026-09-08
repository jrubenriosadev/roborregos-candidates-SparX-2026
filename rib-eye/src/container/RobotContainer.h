#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "Arduino.h"
#include "subsystems/drive/Drive.h"
#include "components/ultrasonic/Ultrasonic.h"
#include "constants/pinsConfig.h"

enum class DriveState {
    FORWARD,
    STOP_1,
    TURN_RIGHT,
    STOP_2
};

class RobotContainer {
    public:
        RobotContainer();

        void init();
        void update();
    
    private:
        Drive drive;
        Ultrasonic ultrasonic;

        DriveState currentState = DriveState::FORWARD;
        unsigned long stateTimer = 0;

        static constexpr uint32_t FORWARD_TIME_MS = 1000;
        static constexpr uint32_t STOP_TIME_MS    = 100;
        static constexpr uint32_t TURN_TIME_MS    = 150;
};


#endif