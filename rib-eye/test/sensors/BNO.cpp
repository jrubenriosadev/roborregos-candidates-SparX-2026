#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(Pins::LedRGB::R_PIN, OUTPUT);
    pinMode(Pins::LedRGB::G_PIN, OUTPUT);
    pinMode(Pins::LedRGB::B_PIN, OUTPUT);
}

void loop() {
    analogWrite(Pins::LedRGB::R_PIN, 255);
    analogWrite(Pins::LedRGB::G_PIN, 0);
    analogWrite(Pins::LedRGB::B_PIN, 0);
    delay(20);
}
