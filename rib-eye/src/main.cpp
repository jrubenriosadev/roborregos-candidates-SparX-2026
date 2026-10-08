#include <Arduino.h>

#include "container/RobotContainer.hpp"

RobotContainer container;

unsigned long lastPrint = 0;

void setup() {

    Serial.begin(115200);

    while (!Serial && millis() < 3000);

    Wire.begin(21, 22);
    Wire.setClock(100000);

    container.init();

    container.getDrive().stop();
}
void loop() {

    container.update();

    Drive& drive = container.getDrive();

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

    delay(20);
}