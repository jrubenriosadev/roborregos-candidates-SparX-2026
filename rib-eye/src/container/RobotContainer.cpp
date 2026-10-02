#include "RobotContainer.hpp"

const float TOLERANCE = 10.0f;

struct Target {
    ColorDetected type;
    RGB target;
    RGB ledOutput;
};

const Target TABLE[] {
    {ColorDetected::RED, {180.0f, 50.0f, 40.0f}, {255.0f, 0.0f, 0.0f}},
    {ColorDetected::MAGENTA, {150.0f, 60.0f, 55.0f}, {255.0f, 0.0f, 255.0f}},
    {ColorDetected::CYAN, {50.0f, 110.0f, 95.0f}, {0.0f, 255.0f, 255.0f}},
    {ColorDetected::ORANGE, {150.0f, 75.0f, 35.0f}, {255.0f, 165.0f, 0.0f}},
    {ColorDetected::GREEN, {90.0f, 120.0f, 40.0f}, {0.0f, 128.0f, 0.0f}},
    {ColorDetected::YELLOW, {120.0f, 100.0f, 30.0f}, {255.0f, 255.0f, 0.0f}}
};

const int num_colors = sizeof(TABLE) / sizeof(TABLE[0]);

bool inRange(const RGB& current, const RGB& target, float t) {
    return (fabsf(current.r - target.r) <= t) &&
           (fabsf(current.g - target.g) <= t) &&
           (fabsf(current.b - target.b) <= t);
}

RobotContainer::RobotContainer() 
    : _led(Pins::LedRGB::R_PIN, Pins::LedRGB::G_PIN, Pins::LedRGB::B_PIN),
      _lastColor(ColorDetected::NONE) {}

void RobotContainer::init() {
    Serial.println(" === init === ");
    _led.init();

    if (_bno.init(Wire)) {
        Serial.println("BNO055 :)");
    } else {
        Serial.println("BNO055 :[");
    }

    if(_tcs.init()) {
        _tcs.setLed(true);
        Serial.println("TCS :)");
    } else {
        Serial.println("TCS :[");
    }

    _drive.init();
    Serial.println("Drive :)");
}

void RobotContainer::update() {
    _bno.update();
    _led.update();
}

Drive& RobotContainer::getDrive() { return _drive; }
Bno& RobotContainer::getBno() { return _bno; }
LedRGB& RobotContainer::getLed() { return _led; }
ColorSensor& RobotContainer::getColorSensor() { return _tcs; }

ColorDetected RobotContainer::getColorDetected() {
    RGB c;
    if(_tcs.readAsync(c, 100)) {
        bool matched = false;

        for(int i = 0; i < num_colors; i++) {
            if(inRange(c, TABLE[i].target, TOLERANCE)) {
                if(_led.getMode() != LedRGB::Mode::BLINK) {
                    _led.setColor(TABLE[i].ledOutput);
                }
                _lastColor = TABLE[i].type;
                matched = true;
                return TABLE[i].type;
            }

            if(!matched) {
                if(_led.getMode() != LedRGB::Mode::BLINK) {
                        _led.setColor(TABLE[i].ledOutput);
                    }
                    _lastColor = ColorDetected::NONE;
                    return ColorDetected::NONE;
            }
        }
    }
    return _lastColor;
}