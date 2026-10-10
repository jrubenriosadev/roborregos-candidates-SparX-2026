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

        float leftDist = container.getLeftDistance();
        float rightDist = container.getRightDistance();

        bool leftValid = container.getLeftUlt().isValid();
        bool rightValid = container.getRightUlt().isValid();

        char buffer[120];
        snprintf(buffer, sizeof(buffer),
            "L: %6.2f cm [%s] | R: %6.2f cm [%s]",
            leftDist,
            leftValid ? "OK" : "INVALID",
            rightDist,
            rightValid ? "OK" : "INVALID"
        );

        Serial.println(buffer);
        bluetooth.println(buffer);
    }
}