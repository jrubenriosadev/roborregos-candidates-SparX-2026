#include <Arduino.h>
#include <constants/pinsConfig.h>
#include <components/motor/Motor.h>
// #include <components/led/Led.h>

// Led led(Pins::Led::LED_PIN);
Motor motor1(Pins::Motors::L_IN1, Pins::Motors::L_IN2);
Motor motor2(Pins::Motors::R_IN1, Pins::Motors::R_IN2);

void setup() {
  Serial.begin(115200);

  // led.init();
  initMotors();
}

void loop() {
  /*
  led.on();
  delay(1000);
  led.off();
  delay(1000);
  */
  moveForward(1000);
  turnRight();
}

void initMotors() {
  motor1.init();
  motor2.init();
}

void moveForward(int delayMs) {
  motor1.forward();
  motor2.backward();
  delay(delayMs);
}

void moveBackward(int delayMs) {
  motor2.forward();
  motor1.backward();
  delay(delayMs);
}

void turnRight() {
  motor1.forward();
  motor2.forward();
  delay(150);
}

void turnLeft() {
  motor1.backward();
  motor2.backward();
  delay(150);
}

void stop() {
  motor1.stop();
  motor2.stop();
}