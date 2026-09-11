#ifndef DEF_SQUARE_CONTAINER
#define DEF_SQUARE_CONTAINER

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

class RobotContainerSquareTest {
    public:
        RobotContainerSquareTest();

        void init();
        void update();
    
    private:
        Drive drive;

        DriveState currentState = DriveState::FORWARD;
        unsigned long stateTimer = 0;

        static constexpr uint32_t FORWARD_TIME_MS = 2000;
        static constexpr uint32_t STOP_TIME_MS    = 100;
        static constexpr uint32_t TURN_TIME_MS    = 825;
};


#endif