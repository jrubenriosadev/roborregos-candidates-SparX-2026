#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"

RobotContainer container;
StateMachine stateMachine(container);

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);
    Wire.begin(21,22);
    Wire.setClock(100000);
    container.init();
}

void loop() {
    container.update();
    stateMachine.update();
    delay(20);
}