#include "RobotContainerMpuTest.h"

RobotContainerMpuTest::RobotContainerMpuTest() {}

void RobotContainerMpuTest::init()
{
    if (!mpu.init())
    {
        Serial.println("La mpu son los amigos que hicimos en el camino :(");
    }
}

void RobotContainerMpuTest::update()
{
    MpuData data;
    if (mpu.readAsync(data, 50))
    {
        Serial.print("gX: ");
        Serial.println(data.gX);
        Serial.print("gY: ");
        Serial.println(data.gY);
        Serial.print("gZ: ");
        Serial.println(data.gZ);
        Serial.print("accelX: ");
        Serial.println(data.accelX);
        Serial.print("accelY: ");
        Serial.println(data.accelY);
        Serial.print("accelZ: ");
        Serial.println(data.accelZ);
        Serial.print("roll: ");
        Serial.println(data.roll, 2);
        Serial.print("pitch: ");
        Serial.println(data.pitch, 2);
        Serial.print("yaw: ");
        Serial.println(data.yaw, 2);
    }
}