#include "RobotContainer.hpp"

const float TOLERANCE = 15.0f;

struct Target {
    ColorDetected type;
    RGB target;
};

const Target TABLE[] {
    {ColorDetected::RED, {180.0f, 70.0f, 60.0f}}, // 180 50 40  ------ // 180 70 60
    {ColorDetected::MAGENTA, {160.0f, 60.0f, 60.0f}}, // 150 60 55 ------- // 160 60 60
    {ColorDetected::CYAN, {50.0f, 110.0f, 95.0f}}, // 50 110 95 -------- // 60 115 110
    {ColorDetected::ORANGE, {160.0f, 85.0f, 50.0f}}, // 150 75 35 ------ // 160 85 50
    {ColorDetected::GREEN, {75.0f, 125.0f, 60.0f}}, // 90 120 40 ------ // 75 125 60
    {ColorDetected::YELLOW, {120.0f, 100.0f, 30.0f}} // 120 100 30 ----- // 120 110 50
};

const int num_colors = sizeof(TABLE) / sizeof(TABLE[0]);

bool inRange(const RGB& current, const RGB& target, float t) {
    return (fabsf(current.r - target.r) <= t) && (fabsf(current.g - target.g) <= t) && (fabsf(current.b - target.b) <= t);
}

RobotContainer::RobotContainer() 
    : _lastColor(ColorDetected::NONE),
      _lastRGB{0.0f, 0.0f, 0.0f},
      _leftUltra(Pins::Ultrasonic::kLeftEcho, Pins::Ultrasonic::kLeftTrigger, Pins::Ultrasonic::maxRange),
      _rightUltra(Pins::Ultrasonic::kRightEcho, Pins::Ultrasonic::kRightTrigger, Pins::Ultrasonic::maxRange),
      _tofL(Pins::ToF::kLeftXShut, Pins::ToF::kLeftAddr),
      _tofR(Pins::ToF::kRightXShut, Pins::ToF::kRightAddr),
      _lastUltraToggle(0),
      _readToggle(false) {}

void RobotContainer::init() {
    //TODO: probar el i2c
    for (int attempt = 0; attempt < 5; ++attempt) {
        _tofL.powerOff();
        _tofR.powerOff();
        delay(20);
        const bool okL = _tofL.init(Wire1);
        delay(15);
        const bool okR = _tofR.init(Wire1);
        delay(15);
        if (okL && okR) break;
        //if (okR) break;
    }

    if (_bno.init(Wire)) {
        _bno.resetHeading();
    }

    if (_tcs.init()) {
        _tcs.setLed(true);
    }
    _drive.init();

    _leftUltra.init();
    _rightUltra.init();
}

void RobotContainer::update() {
    _bno.update();
    _tofL.update();
    _tofR.update();

    unsigned long c = millis();
    if (c - _lastUltraToggle >= 20) {
        _lastUltraToggle += 20;
        
        if (_readToggle) {
            float d = _leftUltra.readDistance();
            _leftFilter.push(d, _leftUltra.isValid());
        } else {
            float d = _rightUltra.readDistance();
            _rightFilter.push(d, _rightUltra.isValid());
        }
        _readToggle = !_readToggle;
    }
}

Drive& RobotContainer::getDrive() { return _drive; }
Bno& RobotContainer::getBno() { return _bno; }
ColorSensor& RobotContainer::getColorSensor() { return _tcs; }
Ultrasonic& RobotContainer::getLeftUlt() { return _leftUltra; }
Ultrasonic& RobotContainer::getRightUlt() { return _rightUltra; }

float RobotContainer::getLeftDistance() const { return _leftFilter.valid() ? _leftFilter.value() : Pins::Ultrasonic::maxRange; }
float RobotContainer::getRightDistance() const { return _rightFilter.valid() ? _rightFilter.value() : Pins::Ultrasonic::maxRange; }

uint16_t RobotContainer::getTofLeftMm() const { return _tofL.getDistance(); }
uint16_t RobotContainer::getTofRightMm() const { return _tofR.getDistance(); }
bool RobotContainer::tofReady() const { return _tofL.isUp() && _tofR.isUp(); }
//bool RobotContainer::tofReady() const { return _tofR.isUp(); }

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

bool RobotContainer::frontWall() const {
    if (!tofReady()) return true;
    return _tofL.getDistance() < DriveConfig::kFWallmm || _tofR.getDistance() < DriveConfig::kFWallmm;
}

bool RobotContainer::leftWall() const {
    return getLeftDistance() < DriveConfig::kSWallcm;
}

bool RobotContainer::rightWall() const {
    return getRightDistance() < DriveConfig::kSWallcm;
}