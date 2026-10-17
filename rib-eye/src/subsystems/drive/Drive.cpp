#include "Drive.hpp"
#include "../include/constants/driveConfig.h"
#include <math.h>

namespace Cfg = DriveConfig;

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
      _steerPID(Cfg::kYawKp, Cfg::kYawKi, Cfg::kYawKd, -255.0f, 255.0f),
      _turnPID(Cfg::kSteerKp, 0.0f, Cfg::kSteerKd, -255.0f, 255.0f),
      _targetYaw(0.0f),
      _targetDistance(Cfg::kTSize),
      _wallErrFilt(0.0f),
      _settleCount(0),
      _apprStart(0),
      _apprT(0),
      _apprPulse(0),
      _apprDriving(false),
      _backDistance(0.0f) {}

void Drive::init() {
    _leftMotor.init();
    _rightMotor.init();
    _steerPID.setAngleWrapping(true);
    _turnPID.setAngleWrapping(true);
}

void Drive::stop() {
    _leftMotor.stop();
    _rightMotor.stop();
}

void Drive::setOpenLoop(int leftSpeed, int rightSpeed) {
    _leftMotor.move(leftSpeed);
    _rightMotor.move(rightSpeed);
}

void Drive::prepApproach() {
    stop();
    _apprStart = millis();
    _apprT = millis();
    _apprDriving = false;
}

bool Drive::approachFront(float mm) {
    const unsigned long now = millis();

    if (now - _apprStart > Cfg::kApproachMillis) {
        stop();
        return true;
    }

    if (_apprDriving) {
        if (now - _apprT < _apprPulse) return false;
        stop();
        _apprDriving = false;
        _apprT = now;
        return false;
    }

    if (now - _apprT < Cfg::kApproachPauseMs) return false;

    if (mm > Cfg::kApproachMaxMm) {
        stop();
        return true;
    }

    const float err = mm - Cfg::kTurnDist;
    if (fabsf(err) <= Cfg::kApproachTolMm) {
        stop();
        return true;
    }

    const int p = err > 0 ? (int)Cfg::kApproachPwm : -(int)Cfg::kApproachPwm;
    _apprPulse = constrain((unsigned long)(fabsf(err) * Cfg::kApproachMsPerMm), Cfg::kApproachPulseMinMs, Cfg::kApproachPulseMaxMs);
    setOpenLoop(p, p);
    _apprDriving = true;
    _apprT = now;
    return false;
}

void Drive::prepAngle(float deg_yawAngle) {
    _steerPID.reset();
    _turnPID.reset();
    _settleCount = 0;
    _targetYaw = deg_yawAngle * Cfg::kDegToRad;
}

bool Drive::turnToAngle(float currentYawDeg) {
    if (isnan(currentYawDeg)) {
        stop();
        return false;
    }

    const float out = _turnPID.update(currentYawDeg * Cfg::kDegToRad, _targetYaw);
    const float err = _turnPID.getErr();
    const float errDeg = fabsf(err) / Cfg::kDegToRad;

    if (errDeg < Cfg::kSteerTol) {
        stop();
        if (++_settleCount >= Cfg::kSteerST) {
            _settleCount = 0;
            return true;
        }
        return false;
    }

    _settleCount = 0;

    float cmd = constrain(out, -Cfg::kSteerMax, Cfg::kSteerMax);
    if (fabsf(cmd) < Cfg::kSteerMin) {
        cmd = copysignf(Cfg::kSteerMin, err);
    }

    setOpenLoop((int)cmd, (int)-cmd);
    return false;
}

void Drive::prepMoveTile(float targetDistanceMeters, float targetYawDeg) {
    _leftMotor.resetEncoder();
    _rightMotor.resetEncoder();
    _steerPID.reset();
    _wallErrFilt = 0.0f;

    _targetDistance = targetDistanceMeters;
    _targetYaw = targetYawDeg * Cfg::kDegToRad;
}

bool Drive::moveTile(float currentYaw, float distLeft, float distRight, float baseSpeed) {
    if (isnan(currentYaw)) currentYaw = 0.0f;

    //const float l = _leftMotor.getDistanceMeters();
    const float r = _rightMotor.getDistanceMeters() * Cfg::kRScale;
    const float l = r;
    const float remaining = _targetDistance - (l + r) * 0.5f;

    if (remaining <= Cfg::kStopTol) {
        stop();
        return true;
    }

    float speed = baseSpeed;
    if (remaining < Cfg::kSlow) {
        speed = fmaxf(Cfg::kMinOutput, baseSpeed * (remaining / Cfg::kSlow));
    }

    const float yawRad = currentYaw * Cfg::kDegToRad;
    const float baseYawErr = fabsf(_steerPID.CWErr(_targetYaw, yawRad));

    float wallOffset = 0.0f;
    const bool okL = distLeft > 0.0f && distLeft < Cfg::kUMax;
    const bool okR = distRight > 0.0f && distRight < Cfg::kUMax;
    const bool okY = baseYawErr < Cfg::kUMaxYawErrDeg * Cfg::kDegToRad;

    bool wall = false;
    float err = 0.0f;

    if (Cfg::kUltra && okY) {
        if (okL && okR) {
            err = distRight - distLeft;
            wall = true;
        } else if (okL) {
            err = 2.0f * (Cfg::kUTarget - distLeft);
            wall = true;
        } else if (okR) {
            err = 2.0f * (distRight - Cfg::kUTarget);
            wall = true;
        }
    }

    if (wall) {
        const float wallErr = constrain(err, -Cfg::kUErrClamp, Cfg::kUErrClamp);
        _wallErrFilt += Cfg::kUFilterA * (wallErr - _wallErrFilt);

        float e = _wallErrFilt;
        if (fabsf(e) < Cfg::kUDeadband) {
            e = 0.0f;
        } else {
            e -= (e > 0.0f ? Cfg::kUDeadband : -Cfg::kUDeadband);
        }

        const float maxOffset = Cfg::kUMaxOffDeg * Cfg::kDegToRad;
        wallOffset = constrain(e * Cfg::kUDegPcm * Cfg::kDegToRad, -maxOffset, maxOffset);
    } else {
        _wallErrFilt *= Cfg::kUOffsetDec;
    }

    const float imuOut = _steerPID.update(yawRad, _targetYaw + wallOffset);
    const float imuSteer = Cfg::kImu ? imuOut : 0.0f;
    const float encSteer = Cfg::kEnc ? (-Cfg::kEncKp * (l - r)) : 0.0f;

    const float totalSteer = constrain(encSteer + imuSteer, -Cfg::kMaxSteer, Cfg::kMaxSteer);

    setOpenLoop(constrain((int)(speed + totalSteer + Cfg::kTrim), 0, 255), constrain((int)(speed - totalSteer), 0, 255));
    return false;
}

void Drive::prepMoveBack(float distanceMeters, float targetYawDeg) {
    _leftMotor.resetEncoder();
    _rightMotor.resetEncoder();
    _steerPID.reset();

    _backDistance = distanceMeters;
    _targetYaw = targetYawDeg * Cfg::kDegToRad;
}

bool Drive::moveBack(float currentYaw) {
    if (isnan(currentYaw)) currentYaw = 0.0f;

    const float traveled = -_rightMotor.getDistanceMeters() * Cfg::kRScale;
    if (_backDistance - traveled <= Cfg::kBackTol) {
        stop();
        return true;
    }

    const float yawRad = currentYaw * Cfg::kDegToRad;
    const float imuOut = _steerPID.update(yawRad, _targetYaw);
    const float steer = Cfg::kImu ? constrain(imuOut, -Cfg::kMaxSteer, Cfg::kMaxSteer) : 0.0f;

    setOpenLoop(constrain((int)(-Cfg::kBackPwm + steer), -255, 0), constrain((int)(-Cfg::kBackPwm - steer), -255, 0));
    return false;
}

long Drive::getLeftEncoder() const { return _leftMotor.getEncoder(); }
long Drive::getRightEncoder() const { return _rightMotor.getEncoder(); }
float Drive::getLeftDistance() const { return _leftMotor.getDistanceMeters(); }
float Drive::getRightDistance() const { return _rightMotor.getDistanceMeters(); }