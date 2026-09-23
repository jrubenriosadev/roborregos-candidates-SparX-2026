#ifndef DEF_TUNINGMODE
#define DEF_TUNINGMODE

#include "../container/RobotContainer.hpp"

class TuningMode {
    public:
        TuningMode(RobotContainer& container);
        void run();
        void processSerialCmd();
        void printTelemetry();
    
    private:
        RobotContainer& container;
        float kp, ki, kd;
        float setpoint;
        bool tuningFM; // FOrward mode

};

#endif