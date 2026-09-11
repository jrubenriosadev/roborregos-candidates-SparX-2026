#include "Mpu.h"

Mpu::Mpu()
    : isInit(false),
      lastReadTime(0),
      lastData{0,0,0,0,0,0} {}

bool Mpu::init() {
    Wire.begin();
    mpu.initialize();

    if(mpu.testConnection()) {
        isInit = true;
        return true;
    }

    isInit = false;
    return false;
}

bool Mpu::readAsync(MpuData &data, unsigned long intervalMs) {
    if(!isInit) return false;
    unsigned long currentMillis = millis();
    if(currentMillis - lastReadTime >= intervalMs){
        lastReadTime = currentMillis;

        mpu.getMotion6(&lastData.accelX, &lastData.accelY, &lastData.accelZ, &lastData.gX, &lastData.gY, &lastData.gZ);
        lastData.temp = mpu.getTemperature();

        data = lastData;
        return true;

    }
    data = lastData;
    return false;
}