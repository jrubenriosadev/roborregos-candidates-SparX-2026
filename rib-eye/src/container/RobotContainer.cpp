#include "RobotContainer.hpp"

const float TOLERANCE = 15.0f;

struct Target {
    ColorDetected type;
    RGB target;
};

const Target TABLE[] {
    {ColorDetected::RED, {180.0f, 50.0f, 40.0f}},
    {ColorDetected::MAGENTA, {150.0f, 60.0f, 55.0f}},
    {ColorDetected::CYAN, {50.0f, 110.0f, 95.0f}},
    {ColorDetected::ORANGE, {150.0f, 75.0f, 35.0f}},
    {ColorDetected::GREEN, {90.0f, 120.0f, 40.0f}},
    {ColorDetected::YELLOW, {120.0f, 100.0f, 30.0f}}
};

const int num_colors = sizeof(TABLE) / sizeof(TABLE[0]);

bool inRange(const RGB& current, const RGB& target, float t) {
    return (fabsf(current.r - target.r) <= t) &&
           (fabsf(current.g - target.g) <= t) &&
           (fabsf(current.b - target.b) <= t);
}

RobotContainer::RobotContainer() 
    : _lastColor(ColorDetected::NONE),
      _lastRGB{0.0f, 0.0f, 0.0f},
      _leftUltra(35, 14, 10.0f),
      _rightUltra(34, 12, 10.0f),
      _distLeft(10.0f),
      _distRight(10.0f),
      _lastUltraToggle(0),
      _readToggle(false) {}

void RobotContainer::init() {
    if (_bno.init(Wire)) {
        _bno.resetHeading();
    }

    if (_tcs.init()) {
        _tcs.setLed(true);
    }

    _leftUltra.init();
    _rightUltra.init();
    _drive.init();
}

void RobotContainer::update() {
    _bno.update();

    unsigned long c = millis();
    if (c - _lastUltraToggle >= 40) {
        _lastUltraToggle = c;
        if (_readToggle) {
            _distLeft = _leftUltra.readDistance();
        } else {
            _distRight = _rightUltra.readDistance();
        }
        _readToggle = !_readToggle;
    }
}

Drive& RobotContainer::getDrive() { return _drive; }
Bno& RobotContainer::getBno() { return _bno; }
ColorSensor& RobotContainer::getColorSensor() { return _tcs; }
Ultrasonic& RobotContainer::getLeftUlt() { return _leftUltra; }
Ultrasonic& RobotContainer::getRightUlt() { return _rightUltra; }

float RobotContainer::getLeftDistance() const { return _distLeft; }
float RobotContainer::getRightDistance() const { return _distRight; }

ColorDetected RobotContainer::getColorDetected() {
    RGB c;
    if (_tcs.readAsync(c, 100)) {
        _lastRGB = c;
        
        for (int i = 0; i < num_colors; i++) {
            if (inRange(c, TABLE[i].target, TOLERANCE)) {
                _lastColor = TABLE[i].type;
                return TABLE[i].type;
            }
        }
        _lastColor = ColorDetected::NONE;
        return ColorDetected::NONE;
    }
    return _lastColor;
}

RGB RobotContainer::getLastRGB() const {
    return _lastRGB;
}