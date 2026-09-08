#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t echoPin, uint8_t triggerPin)
    : echoPin(echoPin), triggerPin(triggerPin), lastDistance(-1.0f), lastReadTime(0) {}

void Ultrasonic::init() {
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(triggerPin, LOW);
}

float Ultrasonic::readDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(4);
    
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH, 15000);

    if (duration == 0 || duration < 116) { 
        lastDistance = -1.0f;
    } else {
        lastDistance = (duration * 0.0343f) / 2.0f;
    }

    return lastDistance;
}

bool Ultrasonic::readAsync(float &outDistance, unsigned long intervalMs) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastReadTime >= intervalMs) {
        lastReadTime = currentMillis;
        outDistance = readDistance();
        return true;
    }
    return false;
}