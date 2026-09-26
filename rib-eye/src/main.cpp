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
}