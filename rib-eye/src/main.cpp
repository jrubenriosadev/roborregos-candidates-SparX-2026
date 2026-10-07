#include <Arduino.h>
#include "container/RobotContainer.hpp"

RobotContainer container;

unsigned long lastPrintTime = 0;
const unsigned long PRINT_INTERVAL = 100;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    container.init();
}

void loop() {
    container.update();
    if(millis() - lastPrintTime >= PRINT_INTERVAL) {
        lastPrintTime = millis();

        Serial.print("L: ");
        Serial.print(container.getLeftDistance());
        Serial.print(" | ");
        Serial.print("R: ");
        Serial.println(container.getRightDistance());
    }
    /*

    if (millis() - lastPrintTime >= PRINT_INTERVAL) {
        lastPrintTime = millis();

        float relativeYaw = container.getBno().getRelativeYaw();
        float quatYaw = container.getBno().getQuatYaw();
        imu::Vector<3> acc = container.getBno().getLinealAcc();

        uint8_t sys = 0, gyro = 0, accel = 0, mag = 0;
        container.getBno().getCalibration(&sys, &gyro, &accel, &mag);

        Serial.print("Yaw Relativo: ");
        Serial.print(relativeYaw, 2);
        Serial.print("° | Quat Yaw: ");
        Serial.print(quatYaw, 2);
        Serial.print("° | AccX: ");
        Serial.print(acc.x(), 2);
        Serial.print(" m/s² | Calib [G:");
        Serial.print(gyro);
        Serial.println("]");
    }*/
}
/*
#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"

RobotContainer container;

enum States2 {
    PREP,
    MOVE,
    DELAY
};

States2 s = States2::PREP;
unsigned long pst = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin(21, 22);
    Wire.setClock(100000);

    container.init();
    Serial.println("¡Esta vivo!");
}

void loop() {
    container.update();

    switch(s) {
        case States2::PREP:
            container.getBno().resetHeading();
            
            container.getDrive().prepMoveSraight(0.5f, 0.0f);
            
            s = States2::MOVE;
            break;

        case States2::MOVE: {
            float relativeYaw = container.getBno().getRelativeYaw();

            if(container.getDrive().moveStraight(relativeYaw)) {
                container.getDrive().stop();
                pst = millis();
                s = States2::DELAY;
            }
            break;
        }

        case States2::DELAY:
            if(millis() - pst >= 1000) {
                s = States2::PREP;
            }
            break;
    }

    delay(20);
}*/
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