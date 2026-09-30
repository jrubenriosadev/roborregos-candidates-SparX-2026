#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"
#include "components/color-sensor/ColorSensor.h"
#include "components/led-rgb/ledRGB.hpp"

const float TOLERANCE = 10.0f;

struct Target {
    const char* n;
    RGB target;
    RGB legOutput;
};

const Target TABLE[] {
    {"Rojo", {180.0f, 50.0f, 40.0f}, {255.0f, 0.0f, 0.0f}},
    {"MAGENTA", {150.0f, 60.0f, 55.0f}, {255.0f, 0.0f, 255.0f}},
    {"CYAN", {50.0f, 110.0f, 95.0f}, {0.0f, 255.0f, 255.0f}},
    {"NARANJA", {150.0f, 75.0f, 35.0f}, {255.0f, 165.0f, 0.0f}},
    {"VERDE", {90.0f, 120.0f, 40.0f}, {0.0f, 128.0f, 0.0f}},
    {"AMARILLO", {120.0f, 100.0f, 30.0f}, {255.0f, 255.0f, 0.0f}}
};

const int num_colors = sizeof(TABLE) / sizeof(TABLE[0]);

ColorSensor tcs;
LedRGB led(Pins::LedRGB::R_PIN, Pins::LedRGB::G_PIN, Pins::LedRGB::B_PIN);

bool inRange(const RGB& current, const RGB& target, float t) {
    return (fabsf(current.r - target.r) <= t) &&
           (fabsf(current.g - target.g) <= t) &&
           (fabsf(current.b - target.b) <= t);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    led.init();

    Wire.begin(21, 22);
    Wire.setClock(100000);

    if(!tcs.init()) {
        Serial.print("Error: El TCS son los amigos que hicimos en el camino...");
        while(1);
    }

    tcs.setLed(true);
    Serial.println("INIT!!");
}

void loop() {
    led.update();

    RGB c;
    if(tcs.readAsync(c, 100)) {
        bool matched = false;

        for(int i = 0; i < num_colors; i++) {
            if(inRange(c, TABLE[i].target, TOLERANCE)) {
                led.setColor(TABLE[i].legOutput);
                
                Serial.print("Detected -> {");
                Serial.print(c.r, 1);
                Serial.print(", ");
                Serial.print(c.g, 1);
                Serial.print(", ");
                Serial.print(c.b, 1);
                Serial.println("}");
                
                matched = true;
                break;
            }
        }

        if(!matched) {
            led.off();
            Serial.print("Nada -> {");
            Serial.print(c.r, 1);
            Serial.print(", ");
            Serial.print(c.g, 1);
            Serial.print(", ");
            Serial.print(c.b, 1);
            Serial.println("}");
        }
    }
    delay(20);
}