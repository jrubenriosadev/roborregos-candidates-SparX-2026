#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"
#include "tuning/TuningMode.hpp"

RobotContainer container;
//StateMachine stateMachine(container);
TuningMode tun;

void setup() {
    Serial.begin(115200);
    container.init();
}

void loop() {
    //stateMachine.update();
    tun.run();
}