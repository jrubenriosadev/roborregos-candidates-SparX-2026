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
    Serial.println("[SYSTEM] Robot inicializado correctamente.");
}

void loop() {
    container.update();

    stateMachine.update();
    // pidTest.run();
    // encoderTest.run();
}