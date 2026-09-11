#ifndef DEF_COLOR_SENSOR
#define DEF_COLOR_SENSOR

#include "Arduino.h"
#include "Adafruit_TCS34725.h"

struct RGB {
    float r;
    float g;
    float b;
};

class ColorSensor {
    public:
        ColorSensor();
        bool init();
        void setLed(bool s);

        bool readAsync(RGB &color, unsigned long intervalMS = 100);
    private:
        Adafruit_TCS34725 tcs;
        bool isInit;
        unsigned long lastReadTime;
        RGB lastRGB;
};

#endif