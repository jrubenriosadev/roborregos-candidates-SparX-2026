#ifndef DEF_DRIVECONFIG
#define DEF_DRIVECONFIG

#include "Arduino.h"

namespace DriveConfig {
    constexpr bool kYawCCWP = true;

    // FIELD CONSTANTS
    constexpr float kTileSize = 0.30f;
    constexpr float kStopTolerance = 0.01f;
    constexpr float kSlow = 0.15f;
    constexpr float kMinPWM = 65.0f;
    constexpr float kAccelPWM = 8.0f;
    constexpr float kFrontStop = 1.0f;

    // IMU
    constexpr float kImuWeight = 1.0;
    constexpr float kSyncGains = 2500.0f;
    constexpr float kSyncWeight = 0.2f;

    constexpr float kWallTarget = 7.0f;
    constexpr float kWallKp = 1.5f;
    constexpr float kWallKd = 0.0f;
    constexpr float kWallMaxOffset = 8.0f;
    constexpr float kMaxWallYawErr = 12.0f;
    constexpr float kWallOffsetGone = 0.9f;

    //TEST
    constexpr float kSteerTol = 1.5f;
    constexpr uint8_t kTurnSettleTicks = 5;
    constexpr float kTurnMaxPWM = 250.0f;
    constexpr float kTurnMinPWM = 90.0f;
    constexpr float kFilterAlpha = 0.5f;
    
}

#endif