#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "../components/imu/Bno.hpp"
#include "../subsystems/drive/Drive.hpp"
#include "../components/color-sensor/ColorSensor.h"
#include "../components/led-rgb/ledRGB.hpp"
#include "../include/constants/pinsConfig.h"

enum class ColorDetected {
    NONE,
    RED,
    CYAN,
    YELLOW,
    ORANGE,
    MAGENTA,
    GREEN
};

class RobotContainer {
    public:
        RobotContainer();

        void init();
        void update();

        Drive& getDrive();
        Bno& getBno();
        LedRGB& getLed();
        ColorSensor& getColorSensor();

        ColorDetected getColorDetected();

    private:
        Drive _drive;
        Bno _bno;
        LedRGB _led;
        ColorSensor _tcs;
        ColorDetected _lastColor;
};

#endif