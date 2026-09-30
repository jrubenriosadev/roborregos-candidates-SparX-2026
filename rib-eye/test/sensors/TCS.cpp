#include "Arduino.h"
#include "components/color-sensor/ColorSensor.h"

ColorSensor tcs;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin(21, 22);
    Wire.setClock(100000);

    Serial.println("========= READY TO SET UP =========");
    if(!tcs.init()) {
        Serial.println("Error: El tcs son los amigos que hicimos en el camino...");
        while(1);
    }

    tcs.setLed(true);

    Serial.println("READY =!!!");
}

void loop() {
    RGB color;

    if(tcs.readAsync(color, 300)) {
        Serial.print("{");
        Serial.print(color.r, 1);
        Serial.print("f, ");
        Serial.print(color.g);
        Serial.print("f, ");
        Serial.print(color.b);
        Serial.println("f}");
    }
}