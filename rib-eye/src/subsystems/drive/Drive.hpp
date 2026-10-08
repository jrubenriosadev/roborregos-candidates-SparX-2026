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

        void prepAngle(float deg_yawAngle);
        bool turnToAngle(float current_yawDeg);

        void prepMoveTile(float targetDistanceMeters, float targetYawDeg);
        bool moveTile(float currentYaw, float distLeft, float distRight, float baseSpeed = 140.0f);

        void setSteerGains(float kp, float ki, float kd);

        long getLeftEncoder() const;
        long getRightEncoder() const;

        float getLeftDistance() const;
        float getRightDistance() const;

    private:
        Motor _leftMotor, _rightMotor;
        PIDController _pidLF, _pidRF, _steerPID, _wallPID;
        float _targetYaw, _targetDistance;
};

#endif