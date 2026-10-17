#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "../components/imu/Bno.hpp"
#include "../subsystems/drive/Drive.hpp"
#include "../components/color-sensor/ColorSensor.h"
#include "../components/led-rgb/ledRGB.hpp"
#include "../components/ultrasonic/Ultrasonic.h"
#include "../components/ultrasonic/WallFilter.hpp"
#include "../components/tof-sensor/ToF.hpp"
#include "../components/line-sensor/LineSensor.h"
#include "../include/constants/pinsConfig.h"
#include "../include/constants/driveConfig.h"

enum class ColorDetected {
    NONE,
    RED,
    CYAN,
    YELLOW,
    ORANGE,
    MAGENTA,
    GREEN,
    LIME
};

class RobotContainer {
    public:
        RobotContainer();

        void init();
        void update();

        Drive& getDrive();
        Bno& getBno();
        ColorSensor& getColorSensor();

        Ultrasonic& getLeftUlt();
        Ultrasonic& getRightUlt();

        float getLeftDistance() const;
        float getRightDistance() const;
        WallFilter getLeftWall() const;
        WallFilter getRightWall() const;

        bool frontWall() const;
        bool leftWall() const;
        bool rightWall() const;

        uint16_t getTofLeftMm() const;
        uint16_t getTofRightMm() const;
        bool tofReady() const;
        ToF& getTofL();

        bool lineLeftWhite() const;
        bool lineRightWhite() const;
        uint16_t lineLeftRaw() const;
        uint16_t lineRightRaw() const;

        ColorDetected getColorDetected();
        RGB getLastRGB() const;

    private:
        Drive _drive;
        Bno _bno;
        ColorSensor _tcs;
        ColorDetected _lastColor;
        Ultrasonic _leftUltra, _rightUltra;
        ToF _tofL;
        LineSensor _lineL, _lineR;
        RGB _lastRGB;

        WallFilter _leftFilter, _rightFilter;
        unsigned long _lastUltraToggle;
        bool _readToggle;
};

#endif
