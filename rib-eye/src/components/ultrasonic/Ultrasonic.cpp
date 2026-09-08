#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t echoPin, uint8_t triggerPin)
    : echoPin(echoPin), triggerPin(triggerPin), lastDistance(-1.0f), lastReadTime(0) {}

void Ultrasonic::init(){
    pinMode(echoPin, INPUT);
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
}

float Ultrasonic::readDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    unsigned long d = pulseIn(echoPin, HIGH, 15000);

    if(d == 0) {
        lastDistance = -1.0f;
    } else {
        lastDistance = (d * 0.0343f) / 2.0f;
    }

    return lastDistance;
}

bool Ultrasonic::readAsync(float &outDistnace, unsigned long intervalMs) {
    unsigned long currentMillis = millis();
    if(currentMillis - lastReadTime >= intervalMs) {
        lastReadTime = currentMillis;
        outDistnace = readDistance();
        return true;
    }
    return false;
}