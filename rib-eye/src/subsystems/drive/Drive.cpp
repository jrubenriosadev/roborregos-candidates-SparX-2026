#include "Drive.hpp"
#include <math.h>

Drive::Drive() 
    : _leftMotor(
        Pins::Motors::kLeftMotorIN1, Pins::Motors::kLeftMotorIN2, Pins::Motors::kLeftPWM,
        Pins::Motors::kLeftEncoderA, Pins::Motors::kLeftEncoderB, Pins::Motors::kLeftPPR,
        Pins::Motors::kWheelDiameter, Pins::Motors::leftInversion
      ),
      _rightMotor(
        Pins::Motors::kRightMotorIN1, Pins::Motors::kRightMotorIN2, Pins::Motors::kRightPWM,
        Pins::Motors::kRightEncoderA, Pins::Motors::kRightEncoderB, Pins::Motors::kRightPPR,
        Pins::Motors::kWheelDiameter, Pins::Motors::rightInversion
      ),
      _pidLF(1800.0f, 0.0f, 0.5f, -255.0f, 255.0f),
      _pidRF(1800.0f, 0.0f, 0.5f, -255.0f, 255.0f),
      _steerPID(380.0f, 0.0f, 1.5f, -255.0f, 255.0f),
      _wallPID(12.0f, 0.0f, 0.8f, -100.0f, 100.0f),
      _targetYaw(0.0f),
      _targetDistance(0.30f) {}

void Drive::init() {
    pinMode(Pins::Motors::kStandBy, OUTPUT);
    digitalWrite(Pins::Motors::kStandBy, HIGH);

    _leftMotor.init();
    _rightMotor.init();
    _steerPID.setAngleWrapping(true);
}

void Drive::stop() {
    _leftMotor.stop();
    _rightMotor.stop();
}

void Drive::setOpenLoop(int leftSpeed, int rightSpeed) {
    _leftMotor.move(leftSpeed);
    _rightMotor.move(rightSpeed);
}

void Drive::prepAngle(float deg_yawAngle) {
    _steerPID.reset();
    _wallPID.reset();
    _targetYaw = deg_yawAngle * (M_PI / 180.0f);
}

void Drive::prepMoveTile(float targetDistanceMeters, float targetYawDeg) {
    _leftMotor.resetEncoder();
    _rightMotor.resetEncoder();

    _pidLF.reset();
    _pidRF.reset();
    _steerPID.reset();
    _wallPID.reset();

    _targetDistance = targetDistanceMeters;
    _targetYaw = targetYawDeg * (M_PI / 180.0f);
}

bool Drive::moveTile(float currentYaw, float distLeft, float distRight, float baseSpeed) {
    if (isnan(currentYaw)) {
        currentYaw = 0.0f;
    }

    float currentLeftDist = _leftMotor.getDistanceMeters();
    float currentRightDist = _rightMotor.getDistanceMeters();
    
    float avgDist = fabsf(currentLeftDist - currentRightDist) > 0.10f ? 
                    fmaxf(currentLeftDist, currentRightDist) : 
                    (currentLeftDist + currentRightDist) / 2.0f;

    float remainingDist = _targetDistance - avgDist;

    if (remainingDist <= 0.015f) {
        stop();
        return true;
    }

    float currentSpeed = baseSpeed;
    if (remainingDist < 0.15f) {
        float factor = remainingDist / 0.15f;
        currentSpeed = baseSpeed * factor;
        if (currentSpeed < 65.0f) {
            currentSpeed = 65.0f;
        }
    }

    float encoderError = currentLeftDist - currentRightDist; 
    float encoderSteer = -2500.0f * encoderError;

    float currentYawRad = currentYaw * (M_PI / 180.0f);
    float imuSteer = -_steerPID.update(currentYawRad, _targetYaw);

    float wallSteer = 0.0f;
    const float MAX_WALL_DIST = 10.0f;
    const float TARGET_WALL_DIST = 6.5f;

    bool leftValid  = (distLeft > 0.0f && distLeft <= MAX_WALL_DIST);
    bool rightValid = (distRight > 0.0f && distRight <= MAX_WALL_DIST);

    if (leftValid && rightValid) {
        float wallError = (distLeft - distRight); 
        wallSteer = _wallPID.update(wallError, 0.0f);
    } else if (leftValid) {
        float wallError = (distLeft - TARGET_WALL_DIST);
        wallSteer = _wallPID.update(wallError, 0.0f);
    } else if (rightValid) {
        float wallError = (TARGET_WALL_DIST - distRight);
        wallSteer = _wallPID.update(wallError, 0.0f);
    } else {
        _wallPID.reset();
    }

    float totalSteer = (encoderSteer * 0.7f) + (imuSteer * 0.1f) + (wallSteer * 0.2f);

    int leftSpeed  = constrain((int)(currentSpeed + totalSteer), -255, 255);
    int rightSpeed = constrain((int)(currentSpeed - totalSteer), -255, 255);

    setOpenLoop(leftSpeed, rightSpeed);
    return false;
}