#ifndef DEF_DRIVE
#define DEF_DRIVE

#include "Arduino.h"
#include "constants/pinsConfig.h"
#include "components/motor/Motor.hpp"
#include "components/motor/controller/PIDController.hpp"

class Drive {
    public:
        Drive();

        void init();

        void setOpenLoop(int leftSpeed, int rightSpeed);
        void stop();

        void prepDistance(float m_distance);
        bool moveToDistance();

        void prepAngle(float deg_yawAngle);
        bool turnToAngle(float current_yawDeg);
    
    private:
        Motor _leftMotor, _rightMotor;
        PIDController _pidLF, _pidRF, _steerPID;
        float _targetDistance, _targetYaw;
};

#endif