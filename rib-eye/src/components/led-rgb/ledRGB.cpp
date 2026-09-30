#include "ledRGB.hpp"

LedRGB::LedRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool isCommonAnode)
    : rPin(rPin),
      gPin(gPin),
      bPin(bPin),
      isCommonAnode(isCommonAnode),
      currentColor{0.0f, 0.0f, 0.0f},
      blinkColor{0.0f, 0.0f, 0.0f},
      currentMode(Mode::OFF),
      lastToggleTime(0),
      blinkInterval(500),
      blinkState(false) {}

void LedRGB::init() {
    pinMode(rPin, OUTPUT);
    pinMode(gPin, OUTPUT);
    pinMode(bPin, OUTPUT);
    off();
}

void LedRGB::writePins(uint8_t r, uint8_t g, uint8_t b) {
    if (isCommonAnode) {
        r = 255 - r;
        g = 255 - g;
        b = 255 - b;
    }
    analogWrite(rPin, r);
    analogWrite(gPin, g);
    analogWrite(bPin, b);
}

void LedRGB::setColor(uint8_t r, uint8_t g, uint8_t b) {
    currentMode = Mode::SOLID;
    currentColor = {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)};
    writePins(r, g, b);
}

void LedRGB::setColor(const RGB& color) {
    setColor(static_cast<uint8_t>(color.r), static_cast<uint8_t>(color.g), static_cast<uint8_t>(color.b));
}

void LedRGB::blink(uint8_t r, uint8_t g, uint8_t b, unsigned long intervalMs) {
    currentMode = Mode::BLINK;
    blinkColor = {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)};
    blinkInterval = intervalMs;
    lastToggleTime = millis();
    blinkState = true;
    writePins(r, g, b);
}

void LedRGB::blink(const RGB& color, unsigned long intervalMs) {
    blink(static_cast<uint8_t>(color.r), 
          static_cast<uint8_t>(color.g), 
          static_cast<uint8_t>(color.b), 
          intervalMs);
}

void LedRGB::off() {
    currentMode = Mode::OFF;
    currentColor = {0.0f, 0.0f, 0.0f};
    writePins(0, 0, 0);
}

void LedRGB::update() {
    if (currentMode != Mode::BLINK) {
        return;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - lastToggleTime >= blinkInterval) {
        lastToggleTime = currentMillis;
        blinkState = !blinkState;

        if (blinkState) {
            writePins(static_cast<uint8_t>(blinkColor.r), static_cast<uint8_t>(blinkColor.g), static_cast<uint8_t>(blinkColor.b));
        } else {
            writePins(0, 0, 0);
        }
    }
}