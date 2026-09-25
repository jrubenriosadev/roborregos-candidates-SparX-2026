#include <Arduino.h>
#include "components/imu/Bno.hpp"

Bno bno;

void setup() {
    Serial.begin(115200);
    delay(1000); 

    Wire.end(); 
    delay(50);

    Wire.begin(21, 22);
    Wire.setClock(100000);

    if (!bno.init()) {
        Serial.println("El bno son los amigos que hicimos en el camino");
    } else {
        Serial.println("Esta papupro");
    }
}

void loop() {
    bno.update();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 100) {
        lastPrint = millis();
        if(bno.isUp()) {
            imu::Vector<3> euler = bno.getEuler();   
            Serial.printf("Yaw: %.2f | Pitch: %.2f | Roll: %.2f\n", euler.x(), euler.z(), euler.y());
        }

    }
}