#include <Arduino.h>
#include <constants/pinsConfig.h>
#include <components/Led.h>

Led led(Pins::Led::LED_PIN);

void setup() {
  Serial.begin(115200);

  led.init();
}

void loop() {
  led.on();
  delay(1000);
  led.off();
  delay(1000);
}