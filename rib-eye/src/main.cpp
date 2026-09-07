#include "Arduino.h"
#include "container/RobotContainer.h"

RobotContainer robotContainer;

void setup() {
  Serial.begin(115200);
  robotContainer.init();
}

void loop() {
  robotContainer.update();
}