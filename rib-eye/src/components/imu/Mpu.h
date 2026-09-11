#ifndef DEF_MPU
#define DEF_MPU

#include "Arduino.h"
#include "Wire.h"
#include "MPU6050.h"

struct MpuData {
    int16_t accelX, accelY, accelZ;
    int16_t gX, gY, gZ;
    int16_t temp;
};

class Mpu {
    public:
        Mpu();
        bool init();
        bool readAsync(MpuData &data, unsigned long intervalMs = 50);
    private:
        MPU6050 mpu;
        bool isInit;
        unsigned long lastReadTime;
        MpuData lastData;
};

#endif DEF_MPU
