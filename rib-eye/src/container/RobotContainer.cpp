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
      _lastColor(ColorDetected::NONE),
      _leftUltra(35, 14),
      _rightUltra(34, 12),
      _distLeft(50.0f),
      _distRight(50.0f),
      _lastUltraToggle(0),
      _readToggle(false) {}

void RobotContainer::init() {
    /*
    Serial.println(" === init === ");
    //_led.init();

    if (_bno.init(Wire)) {
        Serial.println("BNO055 :)");

        uint8_t sys = 0, gyro = 0, accel = 0, mag = 0;
        Serial.print("Estabilizando BNO055");
        unsigned long startTime = millis();
        
        while (gyro < 2 && (millis() - startTime < 3000)) {
            _bno.getCalibration(&sys, &gyro, &accel, &mag);
            Serial.print(".");
            delay(200);
        }
        Serial.println();

        _bno.resetHeading();
        Serial.println("BNO055 Heading Reseted");
    } else {
        Serial.println("BNO055 :[");
    }*/

    /*
    if(_tcs.init()) {
        _tcs.setLed(true);
        Serial.println("TCS :)");
    } else {
        Serial.println("TCS :[");
    }*/

   _leftUltra.init();
   _rightUltra.init();

    _drive.init();
    Serial.println("Drive :)");
}

void RobotContainer::update() {
    //_bno.update();
    //_led.update();
    unsigned long c = millis();
    if(c - _lastUltraToggle >= 40) {
        _lastUltraToggle = c;
        if(_readToggle) {
            _distLeft = _leftUltra.readDistance();
        } else {
            _distRight = _rightUltra.readDistance();
        }
        _readToggle = !_readToggle;
    }
}

Drive& RobotContainer::getDrive() { return _drive; }
Bno& RobotContainer::getBno() { return _bno; }
LedRGB& RobotContainer::getLed() { return _led; }
ColorSensor& RobotContainer::getColorSensor() { return _tcs; }
Ultrasonic& RobotContainer::getLeftUlt() { return _leftUltra; }
Ultrasonic& RobotContainer::getRightUlt() { return _rightUltra; }

float RobotContainer::getLeftDistance() const { return _distLeft; }
float RobotContainer::getRightDistance() const { return _distRight; }

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