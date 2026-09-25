#include <Arduino.h>
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

    container.getDrive().prepDistance(0.5f);
}


void loop() {
    container.getDrive().moveToDistance();

    delay(20);
    //container.update();
   // stateMachine.update();
    //container.getDrive().setOpenLoop(80,0);
    // pidTest.run();
    // encoderTest.run();
}