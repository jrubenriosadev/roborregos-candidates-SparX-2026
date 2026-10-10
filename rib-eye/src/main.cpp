#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "BluetoothSerial.h"

BluetoothSerial bluetooth;

RobotContainer container;

constexpr unsigned long kControlPeriod = 20000UL;
unsigned long nextTick = 0;
unsigned long lastPrint = 0;

void setup() {
    Serial.begin(115200);
    bluetooth.begin("RIBEYE");
    while (!Serial && millis() < 3000);

    Wire.begin(21, 22);
    Wire.setClock(100000);

    container.init();
    container.getDrive().stop();

    nextTick = micros();
}

void loop() {
    const unsigned long now = micros();
    if((long)(now-nextTick) < 0) return;
    nextTick += kControlPeriod;
    if((long)(now-nextTick) > (long)kControlPeriod) nextTick = now + kControlPeriod;

    container.update();

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();

        Bno& bno = container.getBno();

        if (!bno.isUp()) {
            const char* err = "el bno fueron los amigos que hicimos en el camino...";
            Serial.println(err);
            bluetooth.println(err);
        } else {
            float relativeYaw = bno.getRelativeYaw();
            imu::Vector<3> euler = bno.getEuler();

            uint8_t sys = 0, gyro = 0, accel = 0, mag = 0;
            bno.getCalibration(&sys, &gyro, &accel, &mag);

            char buffer[160];
            snprintf(buffer, sizeof(buffer),
                "YawRel: %6.2f° | Yaw: %6.2f° | Pitch: %6.2f° | Roll: %6.2f° | Cal [S:%d G:%d A:%d M:%d]",
                relativeYaw,
                euler.x(),
                euler.z(), // Pitch
                euler.y(), // Roll
                sys, gyro, accel, mag
            );

            Serial.println(buffer);
            bluetooth.println(buffer);
        }
    }
}