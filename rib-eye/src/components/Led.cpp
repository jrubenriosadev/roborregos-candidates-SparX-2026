#include "Led.h"

Led::Led(uint8_t pin) : pin(pin) {}

void Led::init() {
    pinMode(pin, OUTPUT);
    off();

}

void Led::on() {
    digitalWrite(pin, HIGH);
}

void Led::off() {
    digitalWrite(pin, LOW);
}