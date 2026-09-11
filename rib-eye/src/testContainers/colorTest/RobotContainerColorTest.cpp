#include "RobotContainerColorTest.h"

RobotContainerColorTest::RobotContainerColorTest() {}

void RobotContainerColorTest::init() {
    tcs.init();
}

void RobotContainerColorTest::update() {
    
    RGB color;

    if(tcs.readAsync(color, 100)) {
        Serial.print("R: ");
        Serial.println(color.r);
        Serial.print("G: ");
        Serial.println(color.g);
        Serial.print("B: ");
        Serial.println(color.b);
    }
}