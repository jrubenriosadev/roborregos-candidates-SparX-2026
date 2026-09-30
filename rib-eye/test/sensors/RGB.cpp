#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"

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