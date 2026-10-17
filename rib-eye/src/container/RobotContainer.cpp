#include "RobotContainer.hpp"

const float TOLERANCE = 15.0f;

struct Target {
    ColorDetected type;
    RGB target;
};

const Target TABLE[] {
    {ColorDetected::RED, {180.0f, 70.0f, 60.0f}}, // 180 50 40  ------ // 180 70 60
    {ColorDetected::MAGENTA, {160.0f, 60.0f, 60.0f}}, // 150 60 55 ------- // 160 60 60
    {ColorDetected::CYAN, {60.0f, 115.0f, 110.0f}}, // 50 110 95 -------- // 60 115 110
    {ColorDetected::ORANGE, {160.0f, 85.0f, 50.0f}}, // 150 75 35 ------ // 160 85 50
    {ColorDetected::GREEN, {75.0f, 125.0f, 60.0f}}, // 90 120 40 ------ // 75 125 60
    {ColorDetected::YELLOW, {120.0f, 110.0f, 50.0f}}, // 120 100 30 ----- // 120 110 50
    {ColorDetected::LIME, {97.0f, 118.0f, 55.0f}} // estimado
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
      _lineL(Pins::Line::kLeft),
      _lineR(Pins::Line::kRight),
      _lastUltraToggle(0),
      _readToggle(false) {}

void RobotContainer::init() {
    if (_tcs.init()) {
        _tcs.setLed(true);
    }

    if (_bno.init(Wire)) {
        _bno.resetHeading();
    }

    _tofL.powerOff();
    delay(10);

    for (int attempt = 0; attempt < 5; ++attempt) {
        if (_tofL.init(Wire1)) {
            break;
        }
        delay(20);
    }

    //_lineL.init();
    //_lineR.init();

    _drive.init();
    _leftUltra.init();
    _rightUltra.init();

    _lastUltraToggle = millis();
}

void RobotContainer::update() {
    _bno.update();
    _tofL.update();
    _lineL.update();
    _lineR.update();

    unsigned long c = millis();
    if (c - _lastUltraToggle >= 40) {
        _lastUltraToggle += 40;
        
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
ToF& RobotContainer::getTofL() { return _tofL; }
bool RobotContainer::tofReady() const { return _tofL.isUp(); }
//bool RobotContainer::tofReady() const { return _tofR.isUp(); }

bool RobotContainer::lineLeftWhite() const { return _lineL.isWhite(); }
bool RobotContainer::lineRightWhite() const { return _lineR.isWhite(); }
uint16_t RobotContainer::lineLeftRaw() const { return _lineL.getRaw(); }
uint16_t RobotContainer::lineRightRaw() const { return _lineR.getRaw(); }

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
    return _tofL.getDistance() < DriveConfig::kFWallmm;
}

bool RobotContainer::leftWall() const {
    return getLeftDistance() < DriveConfig::kSWallcm;
}

bool RobotContainer::rightWall() const {
    return getRightDistance() < DriveConfig::kSWallcm;
}
