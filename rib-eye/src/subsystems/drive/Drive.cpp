#include "Drive.hpp"
#include "../include/constants/driveConfig.h"
#include <math.h>
#include "../../components/shiftreg/ShiftReg.hpp"

namespace Cfg = DriveConfig;

Drive::Drive()
    : _flMotor(
        Pins::Motors::kFLMotorIN1, Pins::Motors::kFLMotorIN2, Pins::Motors::kLeftPWM,
        Pins::Motors::kLeftEncoderA, Pins::Motors::kLeftEncoderB, Pins::Motors::kLeftPPR,
        Pins::Motors::kWheelDiameter, Pins::Motors::leftInversion
      ),
      _frMotor(
        Pins::Motors::kFRMotorIN1, Pins::Motors::kFRMotorIN2, Pins::Motors::kRightPWM,
        Pins::Motors::kRightEncoderA, Pins::Motors::kRightEncoderB, Pins::Motors::kRightPPR,
        Pins::Motors::kWheelDiameter, Pins::Motors::rightInversion
      ),
      _blMotor(
        Pins::Motors::kBLMotorIN1, Pins::Motors::kBLMotorIN2, Pins::Motors::kLeftPWM,
        Pins::kNan, Pins::kNan, Pins::Motors::kLeftPPR,
        Pins::Motors::kWheelDiameter, Pins::Motors::leftInversion
      ),
      _brMotor(
        Pins::Motors::kBRMotorIN1, Pins::Motors::kBRMotorIN2, Pins::Motors::kRightPWM,
        Pins::kNan, Pins::kNan, Pins::Motors::kRightPPR,
        Pins::Motors::kWheelDiameter, Pins::Motors::rightInversion
      ),
      _steerPID(Cfg::kYawKp, Cfg::kYawKi, Cfg::kYawKd, -255.0f, 255.0f),
      _turnPID(Cfg::kSteerKp, 0.0f, Cfg::kSteerKd, -255.0f, 255.0f),
      _targetYaw(0.0f),
      _targetDistance(Cfg::kTSize),
      _wallErrFilt(0.0f) {}

void Drive::init() {
    ShiftReg::init(Pins::ShiftReg::kData, Pins::ShiftReg::kClock, Pins::ShiftReg::kLatch, Pins::ShiftReg::kOE);
    pinMode(Pins::Motors::kStandBy, OUTPUT);
    digitalWrite(Pins::Motors::kStandBy, HIGH);
    _flMotor.init();
    _frMotor.init();
    _blMotor.init();
    _brMotor.init();
    _steerPID.setAngleWrapping(true);
    _turnPID.setAngleWrapping(true);
}

void Drive::stop() {
    _flMotor.stop();
    _frMotor.stop();
    _blMotor.stop();
    _brMotor.stop();
}

void Drive::setOpenLoop(int leftSpeed, int rightSpeed) {
    _flMotor.move(leftSpeed);
    _frMotor.move(rightSpeed);
    _blMotor.move(leftSpeed);
    _brMotor.move(rightSpeed);
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
    _flMotor.resetEncoder();
    _frMotor.resetEncoder();
    _steerPID.reset();
    _wallErrFilt = 0.0f;

    _targetDistance = targetDistanceMeters;
    _targetYaw = targetYawDeg * Cfg::kDegToRad;
}

bool Drive::moveTile(float currentYaw, float distLeft, float distRight, float baseSpeed) {
    if (isnan(currentYaw)) currentYaw = 0.0f;

    const float l = _flMotor.getDistanceMeters();
    const float r = _frMotor.getDistanceMeters() * Cfg::kRScale;
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

    setOpenLoop(constrain((int)(speed + totalSteer + 5), 0, 255), constrain((int)(speed - totalSteer), 0, 255));
    return false;
}

long Drive::getLeftEncoder() const { return _flMotor.getEncoder(); }
long Drive::getRightEncoder() const { return _frMotor.getEncoder(); }
float Drive::getLeftDistance() const { return _flMotor.getDistanceMeters(); }
float Drive::getRightDistance() const { return _frMotor.getDistanceMeters(); }