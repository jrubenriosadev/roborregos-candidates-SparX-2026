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
      _turnPID(Cfg::kTurnKp, 0.0f, Cfg::kTurnKd, -255.0f, 255.0f),
      _targetYaw(0.0f),
      _targetDistance(Cfg::kTileSize),
      _wallErrFilt(0.0f) {}

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

void Drive::prepAngle(float deg_yawAngle) {
    _steerPID.reset();
    _turnPID.reset();
    _settleCount = 0;
    _targetYaw = deg_yawAngle * Cfg::kDeg2Rad;
}

bool Drive::turnToAngle(float currentYawDeg) {
    if (isnan(currentYawDeg)) {
        stop();
        return false;
    }

    const float out = _turnPID.update(currentYawDeg * Cfg::kDeg2Rad, _targetYaw);
    const float err = _turnPID.getErr();
    const float errDeg = fabsf(err) / Cfg::kDeg2Rad;

    if (errDeg < Cfg::kSteerTol) {
        stop();
        if (++_settleCount >= Cfg::kTurnSettleTicks) {
            _settleCount = 0;
            return true;
        }
        return false;
    }

    _settleCount = 0;

    float cmd = constrain(out, -Cfg::kTurnMaxPWM, Cfg::kTurnMaxPWM);
    if (fabsf(cmd) < Cfg::kTurnMinPWM) {
        cmd = copysignf(Cfg::kTurnMinPWM, err);
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
    _targetYaw = targetYawDeg * Cfg::kDeg2Rad;
}

bool Drive::moveTile(float currentYaw, float distLeft, float distRight, float baseSpeed) {
    if (isnan(currentYaw)) currentYaw = 0.0f;

    const float l = _leftMotor.getDistanceMeters();
    const float r = _rightMotor.getDistanceMeters() * Cfg::kRightScale;
    const float remaining = _targetDistance - (l + r) * 0.5f;

    if (remaining <= Cfg::kStopTolerance) {
        stop();
        return true;
    }

    float speed = baseSpeed;
    if (remaining < Cfg::kSlowZone) {
        speed = fmaxf(Cfg::kMinPWM, baseSpeed * (remaining / Cfg::kSlowZone));
    }

    const float yawRad = currentYaw * Cfg::kDeg2Rad;
    const float baseYawErr = fabsf(_steerPID.CWErr(_targetYaw, yawRad));

    float wallOffset = 0.0f;
    const bool wallsOk = Cfg::kUseWall &&
        distLeft > 0.0f && distLeft < Cfg::kWallMaxCm &&
        distRight > 0.0f && distRight < Cfg::kWallMaxCm &&
        baseYawErr < Cfg::kWallMaxYawErrDeg * Cfg::kDeg2Rad;

    if (wallsOk) {
        const float wallErr = constrain(distRight - distLeft, -Cfg::kWallErrClampCm, Cfg::kWallErrClampCm);
        _wallErrFilt += Cfg::kWallFilterAlpha * (wallErr - _wallErrFilt);

        float e = _wallErrFilt;
        if (fabsf(e) < Cfg::kWallDeadbandCm) {
            e = 0.0f;
        } else {
            e -= (e > 0.0f ? Cfg::kWallDeadbandCm : -Cfg::kWallDeadbandCm);
        }

        const float maxOffset = Cfg::kWallMaxOffsetDeg * Cfg::kDeg2Rad;
        wallOffset = constrain(e * Cfg::kWallDegPerCm * Cfg::kDeg2Rad, -maxOffset, maxOffset);
    } else {
        _wallErrFilt *= Cfg::kWallOffsetDecay;
    }

    const float imuOut = _steerPID.update(yawRad, _targetYaw + wallOffset);
    const float imuSteer = Cfg::kUseImu ? imuOut : 0.0f;
    const float encSteer = Cfg::kUseEnc ? (-Cfg::kEncKp * (l - r)) : 0.0f;

    const float totalSteer = constrain(encSteer + imuSteer, -Cfg::kMaxSteer, Cfg::kMaxSteer);

    setOpenLoop(constrain((int)(speed + totalSteer + 5), 0, 255), constrain((int)(speed - totalSteer), 0, 255));
    return false;
}

long Drive::getLeftEncoder() const { return _leftMotor.getEncoder(); }
long Drive::getRightEncoder() const { return _rightMotor.getEncoder(); }
float Drive::getLeftDistance() const { return _leftMotor.getDistanceMeters(); }
float Drive::getRightDistance() const { return _rightMotor.getDistanceMeters(); }