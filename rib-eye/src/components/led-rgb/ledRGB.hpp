#ifndef DEF_LEDRGB
#define DEF_LEDRGB

#include <Arduino.h>
#include "components/color-sensor/ColorSensor.h"

class LedRGB {
public:
    enum class Mode {
        OFF,
        SOLID,
        BLINK
    };

    LedRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool isCommonAnode = false);

    void init();
    void update();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(const RGB& color);
    
    void blink(uint8_t r, uint8_t g, uint8_t b, unsigned long intervalMs);
    void blink(const RGB& color, unsigned long intervalMs);

    void off();

    Mode getMode() const { return currentMode; }

private:
    uint8_t rPin;
    uint8_t gPin;
    uint8_t bPin;
    bool isCommonAnode;

    RGB currentColor;
    RGB blinkColor;
    Mode currentMode;

    unsigned long lastToggleTime;
    unsigned long blinkInterval;
    bool blinkState;

    void writePins(uint8_t r, uint8_t g, uint8_t b);
};

#endif