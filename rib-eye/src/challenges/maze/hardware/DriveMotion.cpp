#include "DriveMotion.hpp"
#include <math.h>
#include "constants/driveConfig.h"
#include "../comp/MazeConfig.hpp"

namespace {
    float normalizeYaw(float a) {
        while (a > 180.0f) a -= 360.0f;
        while (a <= -180.0f) a += 360.0f;
        return a;
    }
}

bool DriveMotion::begin() {
    if (!haveBase_) {
        const float y = c_.getBno().getRelativeYaw();
        if (!isfinite(y)) return false;
        baseYaw_ = y;
        haveBase_ = true;
    }
    return true;
}

float DriveMotion::yawFor(maze::Direction d) const {
    return normalizeYaw(baseYaw_ + 90.0f * static_cast<float>(maze::toIndex(d)));
}

void DriveMotion::startTurn(maze::Direction target) {
    c_.getDrive().prepAngle(yawFor(target));
    mode_ = Mode::TURN;
    t0_ = millis();
}

void DriveMotion::startMove(maze::Direction heading) {
    c_.getDrive().prepMoveTile(DriveConfig::kTSize, yawFor(heading));
    mode_ = Mode::MOVE;
    t0_ = millis();
}

void DriveMotion::abort() {
    c_.getDrive().stop();
    mode_ = Mode::IDLE;
}

maze::NavStatus DriveMotion::update() {
    using maze::NavStatus;
    Drive& drive = c_.getDrive();
    const float yaw = c_.getBno().getRelativeYaw();

    switch (mode_) {
        case Mode::IDLE:
            return NavStatus::IDLE;

        case Mode::TURN:
            if (drive.turnToAngle(yaw)) { mode_ = Mode::SETTLE; t0_ = millis(); return NavStatus::RUNNING; }
            if (millis() - t0_ > maze::MazeConfig::kTurnTimeoutMs) {
                drive.stop();
                mode_ = Mode::IDLE;
                return NavStatus::UNCERTAIN;
            }
            return NavStatus::RUNNING;

        case Mode::SETTLE:
            drive.stop();
            if (millis() - t0_ >= maze::MazeConfig::kTurnSettleMs) { mode_ = Mode::IDLE; return NavStatus::SUCCESS; }
            return NavStatus::RUNNING;

        case Mode::MOVE:
            if (drive.moveTile(yaw, c_.getLeftDistance(), c_.getRightDistance(), DriveConfig::kBSpeed)) {
                drive.stop();
                mode_ = Mode::IDLE;
                return NavStatus::SUCCESS;
            }
            if (millis() - t0_ > maze::MazeConfig::kMoveTimeoutMs) {
                drive.stop();
                mode_ = Mode::IDLE;
                const float progress = 0.5f * (drive.getLeftDistance() + drive.getRightDistance() * DriveConfig::kRScale);
                return progress < maze::MazeConfig::kMinProgressFrac * DriveConfig::kTSize ? NavStatus::FAILED : NavStatus::UNCERTAIN;
            }
            return NavStatus::RUNNING;
    }
    return NavStatus::UNCERTAIN;
}