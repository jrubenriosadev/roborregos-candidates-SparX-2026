#ifndef DEF_TUNINGMODE
#define DEF_TUNINGMODE

#include "../container/RobotContainer.hpp"

class TuningMode {
    public:
        TuningMode(RobotContainer& container); // <-- OBLIGATORIO: Referencia &
        void init();
        void run();

    private:
        RobotContainer& container;             // <-- OBLIGATORIO: Referencia &
        float kp, ki, kd;
        float stepDistance;
        bool tuningFM;

        bool autoStepMode;
        bool isMoving;
        unsigned long pauseStartTime;

        void processSerialCmd();
        void printTelemetry();
        void triggerNextStep();
        void printMenu();
};

#endif