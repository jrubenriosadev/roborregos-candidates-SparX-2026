#include "Section2.hpp"
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
    }

    Section2::Section2(RobotContainer& container)
        : _container(container),
          _state(Section2State::ALIGN),
          _error(Section2Error::NONE),
          _lane(0),
          _target(0),
          _tried(0),
          _progress(0.0f),
          _segBase(0.0f),
          _blockAt(0.0f),
          _sideYaw(0.0f),
          _recentering(false),
          _armed(false),
          _t0(0) {}

    void Section2::begin() {
        _error = Section2Error::NONE;
        _lane = 0;
        _target = 0;
        _tried = 0;
        _progress = 0.0f;
        _segBase = 0.0f;
        _blockAt = 0.0f;
        _recentering = false;
        _armed = false;
        _container.getDrive().prepAngle(Cfg::kTrackYawDeg);
        _t0 = millis();
        _state = Section2State::ALIGN;
    }

    void Section2::abort() {
        _container.getDrive().stop();
        _state = Section2State::FAULT;
    }

    void Section2::fail(Section2Error e) {
        if (_state == Section2State::FAULT) return;
        _error = e;
        _container.getDrive().stop();
        _state = Section2State::FAULT;
    }

    bool Section2::lineBlocked() const {
        return _container.lineLeftWhite() || _container.lineRightWhite();
    }

    int Section2::nextLane() const {
        static const int fromCenter[3] = {0, 1, -1};
        static const int fromRight[3] = {-1, 0, 1};
        static const int fromLeft[3] = {1, 0, -1};
        const int* order = _lane == 0 ? fromCenter : (_lane > 0 ? fromRight : fromLeft);
        for (int i = 0; i < 3; ++i) {
            if (!(_tried & (1 << (order[i] + 1)))) return order[i];
        }
        return kNoLane;
    }

    void Section2::startDrive() {
        _segBase = _progress;
        _container.getDrive().prepMoveTile(Cfg::kTravelM - _progress, Cfg::kTrackYawDeg);
        _t0 = millis();
        _state = Section2State::DRIVE;
    }

    void Section2::startSideTurn() {
        _sideYaw = normalizeYaw(Cfg::kTrackYawDeg + (_target > _lane ? 90.0f : -90.0f));
        _container.getDrive().prepAngle(_sideYaw);
        _t0 = millis();
        _state = Section2State::TURN_SIDE;
    }

    void Section2::update() {
        Drive& drive = _container.getDrive();
        const float yaw = _container.getBno().getRelativeYaw();

        switch (_state) {
            case Section2State::ALIGN:
                if (drive.turnToAngle(yaw)) {
                    drive.stop();
                    startDrive();
                } else if (millis() - _t0 > Cfg::kTurnTimeoutMs) {
                    fail(Section2Error::TURN_TIMEOUT);
                }
                break;

            case Section2State::DRIVE: {
                const float traveled = _segBase + drive.getRightDistance();

                if (_tried && traveled > _blockAt + Cfg::kClearM) _tried = 0;

                if (!_armed && !lineBlocked()) _armed = true;
                const bool nearEnd = traveled + Cfg::kLineAheadM > Cfg::kFieldEndM - Cfg::kLineMaskEndM;

                if (_armed && !nearEnd && lineBlocked()) {
                    drive.stop();
                    _progress = traveled;
                    _blockAt = traveled;
                    _tried |= (uint8_t)(1 << (_lane + 1));

                    const int next = nextLane();
                    if (next == kNoLane) { fail(Section2Error::NO_FREE_LANE); break; }
                    _target = next;

                    drive.prepMoveBack(Cfg::kBackoffM, Cfg::kTrackYawDeg);
                    _progress = fmaxf(0.0f, _progress - Cfg::kBackoffM);
                    _t0 = millis();
                    _state = Section2State::BACKOFF;
                    break;
                }

                if (drive.moveTile(yaw, _container.getLeftDistance(), _container.getRightDistance(), Cfg::kLaneSpeed)) {
                    drive.stop();
                    _progress = Cfg::kTravelM;
                    if (_lane != 0) {
                        // Termino el campo pero en un carril lateral: re-centrar dentro del checkpoint (ya no hay lineas)
                        _target = 0;
                        _recentering = true;
                        startSideTurn();
                    } else {
                        _state = Section2State::DONE;
                    }
                } else if (millis() - _t0 > Cfg::kLaneMoveTimeoutMs) {
                    fail(Section2Error::MOVE_TIMEOUT);
                }
                break;
            }

            case Section2State::BACKOFF:
                if (drive.moveBack(yaw)) {
                    drive.stop();
                    startSideTurn();
                } else if (millis() - _t0 > Cfg::kShiftTimeoutMs) {
                    fail(Section2Error::MOVE_TIMEOUT);
                }
                break;

            case Section2State::TURN_SIDE:
                if (drive.turnToAngle(yaw)) {
                    drive.stop();
                    drive.prepMoveTile(fabsf((float)(_target - _lane)) * Cfg::kLanePitchM, _sideYaw);
                    _t0 = millis();
                    _state = Section2State::SHIFT;
                } else if (millis() - _t0 > Cfg::kTurnTimeoutMs) {
                    fail(Section2Error::TURN_TIMEOUT);
                }
                break;

            case Section2State::SHIFT:
                if (drive.moveTile(yaw, _container.getLeftDistance(), _container.getRightDistance(), Cfg::kLaneSpeed)) {
                    drive.stop();
                    drive.prepAngle(Cfg::kTrackYawDeg);
                    _t0 = millis();
                    _state = Section2State::TURN_BACK;
                } else if (millis() - _t0 > Cfg::kShiftTimeoutMs) {
                    fail(Section2Error::MOVE_TIMEOUT);
                }
                break;

            case Section2State::TURN_BACK:
                if (drive.turnToAngle(yaw)) {
                    drive.stop();
                    _lane = _target;
                    if (_recentering) _state = Section2State::DONE;
                    else startDrive();
                } else if (millis() - _t0 > Cfg::kTurnTimeoutMs) {
                    fail(Section2Error::TURN_TIMEOUT);
                }
                break;

            case Section2State::DONE:
            case Section2State::FAULT:
                drive.stop();
                break;
        }
    }

}
