#include "Mpu.h"
#include "constants/pinsConfig.h"
#include "constants/mpuConstants.h"
#include <math.h>

Mpu::Mpu()
    : isInit(false),
      lastReadTime(0),
      accelOffsetX(0),
      accelOffsetY(0),
      accelOffsetZ(0),
      gyroOffsetX(0),
      gyroOffsetY(0),
      gyroOffsetZ(0),
      hasF_Data(false),
      roll(MpuConfig::kInitialAngle),
      pitch(MpuConfig::kInitialAngle),
      yaw(MpuConfig::kInitialAngle),
      lastData{0, 0, 0, 0, 0, 0, 0,
      MpuConfig::kInitialAngle,
      MpuConfig::kInitialAngle,
      MpuConfig::kInitialAngle} {}

bool Mpu::init() {
    Wire.begin(Pins::Mpu::kSDA, Pins::Mpu::kSCL);
    Wire.setClock(MpuConfig::kI2cClockHz);

    delay(MpuConfig::kInitDelayMs);

    mpu.initialize();

    delay(MpuConfig::kInitDelayMs);

    if (!mpu.testConnection()) {
        isInit = false;
        return false;
    }

    mpu.setSleepEnabled(false);
    mpu.setClockSource(MPU6050_CLOCK_PLL_XGYRO);
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    mpu.setDLPFMode(MPU6050_DLPF_BW_42);
    mpu.setRate(4);

    calibrate();

    lastReadTime = millis();
    isInit = true;

    return true;
}

void Mpu::calibrate() {
    constexpr int64_t kCalibrationSamples = MpuConfig::kCalibrationSamples;

    int64_t sumAccelX = 0;
    int64_t sumAccelY = 0;
    int64_t sumAccelZ = 0;

    int64_t sumGyroX = 0;
    int64_t sumGyroY = 0;
    int64_t sumGyroZ = 0;

    delay(MpuConfig::kInitDelayMs);

    for (int i = 0; i < kCalibrationSamples; ++i) {
        int16_t accelX;
        int16_t accelY;
        int16_t accelZ;

        int16_t gyroX;
        int16_t gyroY;
        int16_t gyroZ;

        mpu.getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);

        sumAccelX += accelX;
        sumAccelY += accelY;
        sumAccelZ += accelZ;

        sumGyroX += gyroX;
        sumGyroY += gyroY;
        sumGyroZ += gyroZ;

        delay(MpuConfig::kCalibrationDelayMs);
    }

    accelOffsetX = sumAccelX / kCalibrationSamples;
    accelOffsetY = sumAccelY / kCalibrationSamples;

    accelOffsetZ = (sumAccelZ / kCalibrationSamples) - MpuConfig::kAccelGravityRaw;

    gyroOffsetX = sumGyroX / kCalibrationSamples;
    gyroOffsetY = sumGyroY / kCalibrationSamples;
    gyroOffsetZ = sumGyroZ / kCalibrationSamples;

    hasF_Data = false;

    roll = MpuConfig::kInitialAngle;
    pitch = MpuConfig::kInitialAngle;
    yaw = MpuConfig::kInitialAngle;
}

bool Mpu::readAsync(MpuData &data, unsigned long intervalMs) {
    if (!isInit) {
        return false;
    }

    const unsigned long currentMillis = millis();

    if (currentMillis - lastReadTime < intervalMs) {
        data = lastData;
        return false;
    }

    const float deltaTime = (currentMillis - lastReadTime) / MpuConfig::kMsToSeconds;

    lastReadTime = currentMillis;

    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;

    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;

    mpu.getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);

    const int16_t filteredAccelX = accelX - accelOffsetX;
    const int16_t filteredAccelY = accelY - accelOffsetY;
    const int16_t filteredAccelZ = accelZ - accelOffsetZ;

    const int16_t filteredGyroX = gyroX - gyroOffsetX;
    const int16_t filteredGyroY = gyroY - gyroOffsetY;
    const int16_t filteredGyroZ = gyroZ - gyroOffsetZ;

    if (!hasF_Data) {
        lastData.accelX = filteredAccelX;
        lastData.accelY = filteredAccelY;
        lastData.accelZ = filteredAccelZ;

        lastData.gX = filteredGyroX;
        lastData.gY = filteredGyroY;
        lastData.gZ = filteredGyroZ;

        roll = atan2f(lastData.accelY,lastData.accelZ) * MpuConfig::kRadToDeg;

        pitch = atan2f(-lastData.accelX, sqrtf(lastData.accelY * lastData.accelY + lastData.accelZ * lastData.accelZ)) * MpuConfig::kRadToDeg;

        hasF_Data = true;
    } else {
        lastData.accelX += (filteredAccelX -lastData.accelX) * MpuConfig::kFilterA;

        lastData.accelY += (filteredAccelY -lastData.accelY) * MpuConfig::kFilterA;

        lastData.accelZ += (filteredAccelZ -lastData.accelZ) * MpuConfig::kFilterA;

        lastData.gX += (filteredGyroX -lastData.gX) * MpuConfig::kFilterA;

        lastData.gY += (filteredGyroY -lastData.gY) * MpuConfig::kFilterA;

        lastData.gZ += (filteredGyroZ -lastData.gZ) * MpuConfig::kFilterA;

        const float accelRoll = atan2f(lastData.accelY,lastData.accelZ) * MpuConfig::kRadToDeg;

        const float accelPitch =
            atan2f(
                -lastData.accelX, 
                sqrtf(lastData.accelY * lastData.accelY + lastData.accelZ * lastData.accelZ)
            ) * MpuConfig::kRadToDeg;

        const float gyroRoll = lastData.gX / MpuConfig::kGyroSensitivity;

        const float gyroPitch = lastData.gY / MpuConfig::kGyroSensitivity;

        roll = MpuConfig::kGyroWeight * (roll + gyroRoll * deltaTime) + MpuConfig::kAccelWeight * accelRoll;

        pitch = MpuConfig::kGyroWeight * (pitch + gyroPitch * deltaTime) + MpuConfig::kAccelWeight * accelPitch;

        yaw += (lastData.gZ / MpuConfig::kGyroSensitivity) * deltaTime;
    }

    lastData.temp = mpu.getTemperature();

    lastData.roll = roll;
    lastData.pitch = pitch;
    lastData.yaw = yaw;

    data = lastData;

    return true;
}