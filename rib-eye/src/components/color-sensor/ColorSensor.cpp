#include "ColorSensor.h"
#include "Wire.h"

ColorSensor::ColorSensor() 
    : tcs(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X),
      isInit(false),
      lastReadTime(0),
      lastRGB{0.0f, 0.0f, 0.0f} {}

bool ColorSensor::init() {
    if(tcs.begin(0x29, &Wire)) {
        isInit = true;
        setLed(true);
        return true;
    }
    return false;
}

void ColorSensor::setLed(bool s) {
    if(!isInit) return;
    tcs.setInterrupt(!s);
}

bool ColorSensor::readAsync(RGB &color, unsigned long intervalMS) {
    if(!isInit) return false;
    unsigned long currentMillis = millis();
    if(currentMillis - lastReadTime >= intervalMS) {
        lastReadTime = currentMillis;

        tcs.getRGB(&lastRGB.r, &lastRGB.g, &lastRGB.b);

        color = lastRGB;
        return true;
    }

    color = lastRGB;
    return false;
}