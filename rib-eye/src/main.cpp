#include "Arduino.h"
#include "testContainers/motorsTest/RobotContainerEncodersTest.h"

RobotContainerEncodersTest container;

void setup() {
  Serial.begin(115200);
  Serial.print("ESP32: ");
  Serial.println(ESP.getChipModel());

  container.init();
}

void loop() {
  container.update();
}