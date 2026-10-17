#include "LineSensor.h"

LineSensor::LineSensor(uint8_t pin)
    : _pin(pin),
      _raw(0),
      _white(false),
      _count(0) {}

void LineSensor::init() {
    pinMode(_pin, INPUT);
}

void LineSensor::update() {
    if (LineConfig::kDigital) {
        _raw = digitalRead(_pin) ? 4095 : 0;
    } else {
        _raw = (uint16_t)analogRead(_pin);
    }

    // Histeresis: para prender hay que pasar el umbral por un lado, para apagar por el otro
    const int thr = LineConfig::kThreshold;
    const int hys = LineConfig::kHysteresis;
    const int level = LineConfig::kWhiteIsHigh ? (int)_raw : -(int)_raw;
    const int limit = LineConfig::kWhiteIsHigh ? thr : -thr;

    const bool candidate = _white ? (level > limit - hys) : (level > limit + hys);

    if (candidate == _white) {
        _count = 0;
        return;
    }

    if (++_count >= LineConfig::kConfirm) {
        _white = candidate;
        _count = 0;
    }
}

bool LineSensor::isWhite() const { return _white; }
uint16_t LineSensor::getRaw() const { return _raw; }
