#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t echoPin, uint8_t triggerPin, float maxDistanceCm)
    : echoPin(echoPin), 
      triggerPin(triggerPin), 
      lastDistance(maxDistanceCm), 
      lastReadTime(0),
      maxDistanceCm(maxDistanceCm) {
    maxTimeoutUs = (unsigned long)(maxDistanceCm * 2.0f / 0.0343f) + 3000UL;
}

void Ultrasonic::init() {
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(triggerPin, LOW);
}

float Ultrasonic::readDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH, maxTimeoutUs);

    if (duration == 0 || duration < 116) { 
        valid = false;
        lastDistance = maxDistanceCm;
    } else {
        valid = true;
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