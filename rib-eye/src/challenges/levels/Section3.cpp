#include "Section3.hpp"
#include "constants/levelsConfig.h"
#include <math.h>

namespace levels {

    namespace Cfg = LevelsConfig;

    namespace {
        float normalizeYaw(float a) {
            while (a > 180.0f) a -= 360.0f;
            while (a <= -180.0f) a += 360.0f;
            return a;
        }

        float yawForColor(ColorDetected c) {
            switch (c) {
                case ColorDetected::ORANGE: return Cfg::kYawUpDeg;
                case ColorDetected::CYAN: return Cfg::kYawRightDeg;
                case ColorDetected::MAGENTA: return Cfg::kYawDownDeg;
                case ColorDetected::YELLOW: return Cfg::kYawLeftDeg;
                default: return NAN;
            }
        }
    }

    Section3::Section3(RobotContainer& container)
        : _container(container),
          _state(Section3State::ALIGN),
          _error(Section3Error::NONE),
          _targetYaw(0.0f),
          _t0(0),
          _samples(0),
          _retries(0),
          _tiles(0) {
        for (int i = 0; i < kColors; ++i) _votes[i] = 0;
    }

    void Section3::begin() {
        _error = Section3Error::NONE;
        _tiles = 0;
        _retries = 0;
        startAlign(Cfg::kYawUpDeg);
    }

    void Section3::abort() {
        _container.getDrive().stop();
        _state = Section3State::FAULT;
    }

    void Section3::fail(Section3Error e) {
        if (_state == Section3State::FAULT) return;
        _error = e;
        _container.getDrive().stop();
        _state = Section3State::FAULT;
    }

    void Section3::startAlign(float yawDeg) {
        _targetYaw = normalizeYaw(yawDeg);
        _container.getDrive().prepAngle(_targetYaw);
        _t0 = millis();
        _state = Section3State::ALIGN;
    }

    void Section3::startReading() {
        for (int i = 0; i < kColors; ++i) _votes[i] = 0;
        _samples = 0;
        _t0 = millis();
        _state = Section3State::READING;
    }

    void Section3::decide() {
        int best = 0;
        for (int i = 1; i < kColors; ++i) if (_votes[i] > _votes[best]) best = i;

        const bool enough = _samples >= Cfg::kColorMinSamples && _votes[best] * 100 >= _samples * Cfg::kColorAgreePct;
        const ColorDetected c = static_cast<ColorDetected>(best);

        if (enough && c == ColorDetected::LIME) {
            _container.getDrive().stop();
            _state = Section3State::DONE;
            return;
        }

        const float yaw = enough ? yawForColor(c) : NAN;
        if (!isnan(yaw)) {
            if (++_tiles > Cfg::kMaxTiles) { fail(Section3Error::TOO_MANY_TILES); return; }
            _retries = 0;
            startAlign(yaw);
            return;
        }

        if (++_retries > Cfg::kColorRetries) { fail(Section3Error::COLOR_UNREADABLE); return; }
        _t0 = millis();
        _state = Section3State::SETTLE;
    }

    void Section3::update() {
        Drive& drive = _container.getDrive();
        const float yaw = _container.getBno().getRelativeYaw();

        switch (_state) {
            case Section3State::ALIGN:
                if (drive.turnToAngle(yaw)) {
                    drive.stop();
                    _t0 = millis();
                    _state = Section3State::TURN_DELAY;
                } else if (millis() - _t0 > Cfg::kTurnTimeoutMs) {
                    fail(Section3Error::TURN_TIMEOUT);
                }
                break;

            case Section3State::TURN_DELAY:
                drive.stop();
                if (millis() - _t0 >= Cfg::kTurnDelayMs) {
                    drive.prepMoveTile(DriveConfig::kTSize, _targetYaw);
                    _t0 = millis();
                    _state = Section3State::MOVE;
                }
                break;

            case Section3State::MOVE:
                if (drive.moveTile(yaw, _container.getLeftDistance(), _container.getRightDistance(), DriveConfig::kBSpeed)) {
                    drive.stop();
                    _t0 = millis();
                    _state = Section3State::SETTLE;
                } else if (millis() - _t0 > Cfg::kMoveTimeoutMs) {
                    fail(Section3Error::MOVE_TIMEOUT);
                }
                break;

            case Section3State::SETTLE:
                drive.stop();
                _container.getColorDetected();
                if (millis() - _t0 >= Cfg::kColorSettleMs) startReading();
                break;

            case Section3State::READING: {
                drive.stop();
                const ColorDetected c = _container.getColorDetected();
                ++_votes[static_cast<int>(c)];
                ++_samples;
                if (millis() - _t0 >= Cfg::kColorWindowMs) decide();
                break;
            }

            case Section3State::DONE:
            case Section3State::FAULT:
                drive.stop();
                break;
        }
    }

}
