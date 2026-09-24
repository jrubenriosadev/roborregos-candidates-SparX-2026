#include <Arduino.h>
#include "components/tof-sensor/ToF.hpp"

#define XSHUT_1 14
#define XSHUT_2 27

#define ADDR_1 0x30
#define ADDR_2 0x31

ToF sensor1(XSHUT_1, ADDR_1);
ToF sensor2(XSHUT_2, ADDR_2);

void setup() {
    Serial.begin(115200);
    Wire.begin(21,22);

    if (!sensor1.init()) {
        Serial.println("Error iniciando Sensor 1");
    }
    if (!sensor2.init()) {
        Serial.println("Error iniciando Sensor 2");
    }
}

void loop() {
    sensor1.update();
    sensor2.update();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 100) {
        lastPrint = millis();

        Serial.printf("S1: %d mm | S2: %d mm\n", sensor1.getDistance(), sensor2.getDistance());
    }
}