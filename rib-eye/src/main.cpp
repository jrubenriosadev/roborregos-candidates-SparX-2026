#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"
#include "BluetoothSerial.h"

BluetoothSerial bluetooth;
RobotContainer container;
StateMachine stateMachine(container);

constexpr unsigned long kControlPeriod = 20000UL;
unsigned long nextTick = 0;
unsigned long lastPrint = 0;

void recoverI2C(uint8_t sdaPin, uint8_t sclPin) {
    pinMode(sdaPin, INPUT_PULLUP);
    pinMode(sclPin, OUTPUT);
    for (int i = 0; i < 10; i++) {
        digitalWrite(sclPin, LOW);
        delayMicroseconds(5);
        digitalWrite(sclPin, HIGH);
        delayMicroseconds(5);
    }
    pinMode(sdaPin, INPUT);
    pinMode(sclPin, INPUT);
}

void setup() {
    Serial.begin(115200);
    bluetooth.begin("RIBEYE");
    while (!Serial && millis() < 3000);

    recoverI2C(21, 22);
    recoverI2C(32, 23);

    Wire.begin(21, 22);
    Wire.setClock(100000);
    Wire.setTimeOut(50);

    Wire1.begin(32, 23);
    Wire1.setClock(100000);
    Wire1.setTimeOut(50);

    container.init();
    container.getDrive().stop();

    nextTick = micros();
}

void loop() {
    const unsigned long now = micros();
    if ((long)(now - nextTick) < 0) return;
    nextTick += kControlPeriod;
    if ((long)(now - nextTick) > (long)kControlPeriod) nextTick = now + kControlPeriod;

    container.update();
    stateMachine.update();

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();

        char buffer[160];
        snprintf(buffer, sizeof(buffer),
                 "Yaw:%7.2f | ToF L:%4u R:%4u mm | US L:%5.1f R:%5.1f cm",
                 container.getBno().getRelativeYaw(),
                 container.getTofLeftMm(), container.getTofRightMm(),
                 container.getLeftDistance(), container.getRightDistance());

        Serial.println(buffer);
        bluetooth.println(buffer);
    }
}