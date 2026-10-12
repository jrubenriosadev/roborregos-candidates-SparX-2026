#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"
#include "BluetoothSerial.h"

BluetoothSerial bluetooth;
RobotContainer container;
StateMachine stateMachine(container);

constexpr unsigned long kCPeriod = 20000UL;
unsigned long nTick = 0;
unsigned long lPrint = 0;

void recover(uint8_t sda, uint8_t scl) {
    pinMode(sda, INPUT_PULLUP);
    pinMode(scl, OUTPUT);
    for (int i = 0; i < 10; i++) {
        digitalWrite(scl, LOW);
        delayMicroseconds(5);
        digitalWrite(scl, HIGH);
        delayMicroseconds(5);
    }
    pinMode(sda, INPUT);
    pinMode(scl, INPUT);
}

void setup() {
    Serial.begin(115200);
    bluetooth.begin("RIBEYE");
    while (!Serial && millis() < 3000);

    recover(21, 22);
    recover(32, 23);

    Wire.begin(21, 22);
    Wire.setClock(100000);
    Wire.setTimeOut(50);

    Wire1.begin(32, 23);
    Wire1.setClock(100000);
    Wire1.setTimeOut(50);

    container.init();
    container.getDrive().stop();

    nTick = micros();
}

void loop() {
    // Esto es para no usar delay(20) al final, es mas preciso y menos bloqueante
    const unsigned long now = micros();
    if ((long)(now - nTick) < 0) return;
    nTick += kCPeriod;
    if ((long)(now - nTick) > (long)kCPeriod) nTick = now + kCPeriod;

    container.update();
    stateMachine.update();

    if (millis() - lPrint >= 250) {
        lPrint = millis();

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