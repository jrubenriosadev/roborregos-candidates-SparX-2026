#include "RobotContainerMpuTest.h"

RobotContainerMpuTest::RobotContainerMpuTest() {}

void RobotContainerMpuTest::init() {
    mpu.init();
}

void RobotContainerMpuTest::update() {
    MpuData data;
    if(mpu.readAsync(data, 50)) {
        Serial.print("gX: ");
        Serial.println(data.gX);
        Serial.print("accelX: ");
        Serial.println(data.accelX);
        Serial.print("accelY: ");
        Serial.println(data.accelY);
        Serial.print("accelZ: ");
        Serial.println(data.accelZ);
    }
}