#include "MpuTestMode.hpp"
#include <Arduino.h>

MpuTestMode::MpuTestMode(RobotContainer& container)
    : _container(container), _lpt(0) {}

void MpuTestMode::run() {
    _container.getDrive().stop();

    if(millis() - _lpt >= 100) {
        _lpt = millis();

        MpuData imu;
        _container.getMpu().readAsync(imu);

        Serial.print("Roll: ");
        Serial.print(imu.roll, 2);
        Serial.print(" | Pitch: ");
        Serial.print(imu.pitch, 2);
        Serial.print(" | Yaw: ");
        Serial.println(imu.yaw, 2);
    }
}