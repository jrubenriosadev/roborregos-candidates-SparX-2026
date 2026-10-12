#ifndef DEF_DRIVECONFIG
#define DEF_DRIVECONFIG

#include "Arduino.h"

namespace DriveConfig {
    constexpr float kDeg2Rad = 3.14159265358979f / 180.0f;

    constexpr float kTileSize = 0.30f;
    constexpr float kBaseSpeed = 150.0f;
    constexpr float kMinPWM = 100.0f;
    constexpr float kSlowZone = 0.10f;
    constexpr float kStopTolerance = 0.02f;
    constexpr float kMaxSteer = 40.0f;

    constexpr float kRightScale = 1.0f;

    constexpr bool kUseEnc = true;
    constexpr bool kUseImu = true;
    constexpr bool kUseWall = true;

    constexpr float kYawKp = 220.0f;
    constexpr float kYawKi = 0.0f;
    constexpr float kYawKd = 6.0f;

    constexpr float kEncKp = 150.0f;

    constexpr float kWallMaxCm = 10.0f;
    constexpr float kWallDeadbandCm = 1.5f;
    constexpr float kWallFilterAlpha = 0.15f;
    constexpr float kWallErrClampCm = 6.0f;
    constexpr float kWallDegPerCm = 1.5f;
    constexpr float kWallMaxOffsetDeg = 5.0f;
    constexpr float kWallMaxYawErrDeg = 10.0f;
    constexpr float kWallOffsetDecay = 0.9f;

    constexpr float kTurnKp = 260.0f;
    constexpr float kTurnKd = 12.0f;
    constexpr float kSteerTol = 2.0f;
    constexpr uint8_t kTurnSettleTicks = 5;
    constexpr float kTurnMaxPWM = 250.0f;
    constexpr float kTurnMinPWM = 120.0f;
    constexpr float kFilterAlpha = 0.5f;
    constexpr float kRightTurnDeg = 90.0f;


    constexpr uint16_t kFrontWallMm = 100;
    constexpr float kSideWallCm = 15.0f;
    constexpr unsigned long kDecideDelayMs = 1000;
    constexpr unsigned long kTurnSettleMs = 300;
    constexpr bool kPreferRight = true;
    constexpr bool kDeadEndUTurn = true;
}

#endif