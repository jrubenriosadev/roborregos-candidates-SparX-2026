#ifndef MPU_CONFIG_H
#define MPU_CONFIG_H

#include <stdint.h>

namespace MpuConfig {
    constexpr uint32_t kI2cClockHz = 400000UL;

    constexpr uint16_t kInitDelayMs = 100;
    constexpr uint16_t kCalibrationDelayMs = 5;

    constexpr uint16_t kCalibrationSamples = 200;

    constexpr int32_t kAccelGravityRaw = 16384;

    constexpr float kGyroSensitivity = 131.0f;

    constexpr float kMsToSeconds = 1000.0f;
    constexpr float kRadToDeg = 57.2957795f;

    constexpr float kFilterA = 0.3f;

    constexpr float kGyroWeight = 0.98f;
    constexpr float kAccelWeight = 0.02f;

    constexpr float kInitialAngle = 0.0f;

    constexpr unsigned long kDefaultReadIntervalMs = 50UL;

}

#endif