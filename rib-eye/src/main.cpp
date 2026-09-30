#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"
#include "components/color-sensor/ColorSensor.h"

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

void setLed(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(Pins::LedRGB::R_PIN, r);
    analogWrite(Pins::LedRGB::G_PIN, g);
    analogWrite(Pins::LedRGB::B_PIN, b);
}

bool inRange(const RGB& current, const RGB& target, float t) {
    return (fabsf(current.r - target.r) <= t) &&
           (fabsf(current.g - target.g) <= t) &&
           (fabsf(current.b - target.b) <= t);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(Pins::LedRGB::R_PIN, OUTPUT);
    pinMode(Pins::LedRGB::G_PIN, OUTPUT);
    pinMode(Pins::LedRGB::B_PIN, OUTPUT);
    setLed(0,0,0);

    Wire.begin(21,22);
    Wire.setClock(100000);

    if(!tcs.init()) {
        Serial.print("Error: El TCS son los amigos que hicimos en el camino...");
        while(1);
    }

    tcs.setLed(true);
    Serial.println("INIT!!");
}

void loop() {
    RGB c;
    if(tcs.readAsync(c, 100)) {
        bool matched = false;

        for(int i = 0; i < num_colors; i++) {
            if(inRange(c, TABLE[i].target, TOLERANCE)) {
                setLed(TABLE[i].legOutput.r, TABLE[i].legOutput.g, TABLE[i].legOutput.b);
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
            setLed(0,0,0);
            Serial.print("Nada -> {");
            Serial.print(c.r, 1);
            Serial.print(", ");
            Serial.print(c.g, 1);
            Serial.print(", ");
            Serial.print(c.b, 1);
            Serial.println("}");
        }
    }
}

/*RobotContainer container;
StateMachine stateMachine(container); 

void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin(21, 22);
    Wire.setClock(100000);

    container.init();
    Serial.println("Rib-eye is aleye!!");
}

void loop() {
    container.update();
    stateMachine.update();
    delay(20);
}
*/
/*#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"
#include "tuning/TuningMode.hpp"
#include "tuning/Encoders/EncoderTestMode.hpp"

RobotContainer container;
StateMachine stateMachine(container); 
// TuningMode pidTest(container);
// EncoderTestMode encoderTest(container);

void setup() {
    Serial.begin(115200);
    delay(1000);

    container.init();

    Serial.println("pepelin");
}


void loop() {
    static unsigned long lastPrint = 0;

    if(millis() - lastPrint >= 250) {
        lastPrint = millis();

        Serial.print(container.getDrive().getLeftDistance());
        Serial.print(" | ");
        Serial.println(container.getDrive().getRightDistance());
    }
    stateMachine.update();
    container.update();
    delay(20);
    
    //container.getDrive().setOpenLoop(80,80);
    // pidTest.run();
    // encoderTest.run();
}*/
/*#include <Arduino.h>
#include "container/RobotContainer.hpp"

RobotContainer robot;

void setup() {
    Serial.begin(115200);

    Wire.begin(21, 22);
    Wire.setClock(100000);

    robot.init();

    robot.getDrive().prepAngle(90.0f);
}

void loop() {
    robot.update();

    if (robot.getBno().isUp()) {
        float currentYaw = robot.getBno().getEuler().x();
        robot.getDrive().turnToAngle(currentYaw);
    } else {
        robot.getDrive().stop();
    }

    delay(20);
}*/