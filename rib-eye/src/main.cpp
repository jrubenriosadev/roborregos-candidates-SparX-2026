#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"

RobotContainer container;
StateMachine stateMachine(container);

void setup() {
    Serial.begin(115200);
    container.init();
}

void loop() {
    stateMachine.update();
}