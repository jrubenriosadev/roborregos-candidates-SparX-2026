#ifndef DEF_DRIVECONFIG
#define DEF_DRIVECONFIG

#include "Arduino.h"

namespace DriveConfig {
    constexpr float kDegToRad = 3.14159265358979f / 180.0f;

    constexpr float kTSize = 0.3f;
    constexpr float kBSpeed = 140.0f;
    constexpr float kMinOutput = 100.0f;
    constexpr float kSlow = 0.10f;
    constexpr float kStopTol = 0.03f;
    constexpr float kMaxSteer = 40.0f;
    constexpr float kTrim = 10.0f;

    constexpr float kRScale = 1.0f;

    constexpr bool kEnc = true;
    constexpr bool kImu = true;
    constexpr bool kUltra = false;

    constexpr float kYawKp = 220.0f;
    constexpr float kYawKi = 0.0f;
    constexpr float kYawKd = 6.0f;

    constexpr float kEncKp = 140.0f;

    constexpr float kUMax = 14.0f;
    constexpr float kUTarget = 9.0f;
    constexpr float kUDeadband = 1.0f;
    constexpr float kUFilterA = 0.3f;
    constexpr float kUErrClamp = 8.0f;
    constexpr float kUDegPcm = 1.5f;
    constexpr float kUMaxOffDeg = 5.0f;
    constexpr float kUMaxYawErrDeg = 15.0f;
    constexpr float kUOffsetDec = 0.9f; // Decay

    constexpr float kSteerKp = 180.0f;
    constexpr float kSteerKd = 12.0f;
    constexpr float kSteerTol = 2.5f;
    constexpr uint8_t kSteerST = 5; // Settle Ticks
    constexpr float kSteerMax = 180.0f;
    constexpr float kSteerMin = 100.0f;
    constexpr float kSteerDeg90 = 90.0f;

    constexpr uint16_t kFWallmm = 100;
    constexpr float kSWallcm = 15.0f; // cm
    constexpr unsigned long kDecideDelayMs = 800;
    constexpr unsigned long kSteerSms = 300; // Settle ms
    constexpr bool kRightPref = true;
    constexpr bool kDeadEnd180 = true;
}

#endif