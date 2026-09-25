#include "Drive.hpp"
#include <math.h>

Drive::Drive() 
    : 
    _leftMotor(
        Pins::Motors::kLeftMotorIN1, 
        Pins::Motors::kLeftMotorIN2, 
        Pins::Motors::kLeftPWM,
        Pins::Motors::kLeftEncoderA, 
        Pins::Motors::kLeftEncoderB,
        Pins::Motors::kLeftPPR,
        Pins::Motors::kWheelDiameter,
        Pins::Motors::leftInversion
    ),
      
    _rightMotor(
        Pins::Motors::kRightMotorIN1, 
        Pins::Motors::kRightMotorIN2, 
        Pins::Motors::kRightPWM,
        Pins::Motors::kRightEncoderA, 
        Pins::Motors::kRightEncoderB,
        Pins::Motors::kRightPPR,
        Pins::Motors::kWheelDiameter,
        Pins::Motors::rightInversion
    ),

    _pidLF(600.0f,0.0f,0.0f,-255.0f,255.0f),

    _pidRF(600.0f,0.0f,0.0f,-255.0f,255.0f),

    _steerPID(100.0f,0.0f,5.0f,-255.0f,255.0f),

    _targetDistance(0.0f),
    
    _targetYaw(0.0f)
    {}

void Drive::init() {
    _leftMotor.init();
    _rightMotor.init();

    _steerPID.setAngleWrapping(true);
}

void Drive::stop() {
    _leftMotor.stop();
    _rightMotor.stop();
}

void Drive::prepDistance(float m_distance) {
    _leftMotor.resetEncoder();
    _rightMotor.resetEncoder();

    _pidLF.reset();
    _pidRF.reset();

    _targetDistance = m_distance;
}

void Drive::setOpenLoop(int leftSpeed, int rightSpeed) {
    _leftMotor.move(leftSpeed);
    _rightMotor.move(rightSpeed);
}

bool Drive::moveToDistance() {
    float currentLeftDistance = _leftMotor.getDistanceMeters();
    float currentRightDistance = _rightMotor.getDistanceMeters();

    float leftSpeed = _pidLF.update(currentLeftDistance, _targetDistance);
    float rightSpeed = _pidRF.update(currentRightDistance, _targetDistance);

    setOpenLoop((int)leftSpeed, (int)rightSpeed);

    float tolerance = 0.05f;
    bool isLeftAt = fabs(_targetDistance - currentLeftDistance) < tolerance;
    bool isRightAt = fabs(_targetDistance - currentRightDistance) < tolerance;

    if(isLeftAt && isRightAt) {
        stop();
        return true;
    }

    return false;
}

void Drive::prepAngle(float deg_yawAngle) {
    _steerPID.reset();
    _targetYaw = deg_yawAngle * (M_PI / 180.0f); // To native rad
}

bool Drive::turnToAngle(float current_yawDeg) {
    float current_yawRad = current_yawDeg * (M_PI / 180.0f);
    float steer = _steerPID.update(current_yawRad, _targetYaw);
    
    setOpenLoop((int)-steer, (int)steer);

    float err = fabs(_steerPID.getErr() * (180.0f/ M_PI));

    float tolerance = 2.0f;
    if(err < tolerance) {
        stop();
        return true;
    }
    return false;
}

void Drive::setLeftForwardGains(float kp, float ki, float kd) {
    _pidLF.setGains(kp,ki,kd);
    _pidRF.setGains(kp,ki,kd);
}

void Drive::setRightForwardGains(float kp, float ki, float kd) {
    _pidRF.setGains(kp,ki,kd);
}

void Drive::setSteerGains(float kp, float ki, float kd) {
    _steerPID.setGains(kp, ki, kd);
}

float Drive::getLeftDistance() const {
    return _leftMotor.getDistanceMeters();
}

float Drive::getRightDistance() const {
    return _rightMotor.getDistanceMeters();
}

long Drive::getLeftTicks() const {
    return _leftMotor.getEncoder();
}

long Drive::getRightTicks() const {
    return _rightMotor.getEncoder();
}