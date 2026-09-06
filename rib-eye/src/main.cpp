#include "Arduino.h"
#include "container/RobotContainer.h"

RobotContainer robotContainer;

void setup() {
  Serial.begin(115200);
  robotContainer.init();
}

void loop() {
  robotContainer.moveForward(1000);
  robotContainer.stop(100);
  robotContainer.turnRight();
  robotContainer.stop(100);
}