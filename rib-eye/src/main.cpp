#include "Arduino.h"
#include "testContainers/mpuTest/RobotContainerMpuTest.h"

RobotContainerMpuTest robotContainer;

void setup() {
  Serial.begin(115200);
  robotContainer.init();
  Serial.print("ESP32: ");
  Serial.println(ESP.getChipModel());
}

void loop() {
  robotContainer.update();
}