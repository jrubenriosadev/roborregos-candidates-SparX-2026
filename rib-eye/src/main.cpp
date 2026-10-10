#include <Arduino.h>

#include "container/RobotContainer.hpp"

RobotContainer container;

constexpr unsigned long kControlPeriod = 20000UL;
unsigned long nextTick = 0;
unsigned long lastPrint = 0;

void setup() {
    Serial.begin(115200);

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

    Drive& drive = container.getDrive();

    drive.setOpenLoop(110,120);

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        Serial.print("L: ");
        Serial.print(drive.getLeftEncoder());
        Serial.print(" | R: ");
        Serial.print(drive.getRightEncoder());
        Serial.print(" | DL: ");
        Serial.print(drive.getLeftDistance(), 4);
        Serial.print(" m | DR: ");
        Serial.print(drive.getRightDistance(), 4);
        Serial.println(" m");
    }
}