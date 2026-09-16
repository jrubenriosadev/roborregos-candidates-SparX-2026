#ifndef DEF_MPU
#define DEF_MPU

#include "Arduino.h"
#include "Wire.h"
#include "MPU6050.h"

struct MpuData {
    int16_t accelX, accelY, accelZ;
    int16_t gX, gY, gZ;
    int16_t temp;
    float roll;
    float pitch;
    float yaw;
};

class Mpu {
    public:
        Mpu();
        bool init();
        bool readAsync(MpuData &data, unsigned long intervalMs = 50);
    private:
        void calibrate();

        MPU6050 mpu;
        bool isInit;
        unsigned long lastReadTime;
        int32_t accelOffsetX;
        int32_t accelOffsetY;
        int32_t accelOffsetZ;
        int32_t gyroOffsetX;
        int32_t gyroOffsetY;
        int32_t gyroOffsetZ;
        bool hasF_Data;
        float roll;
        float pitch;
        float yaw;
        MpuData lastData;
};

#endif
