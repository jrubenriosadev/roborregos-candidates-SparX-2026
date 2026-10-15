#include "Navigator.hpp"

namespace maze {

    Navigator::Navigator(const Maze& maze, MotionController& motion)
        : maze_(maze), motion_(motion), pose_(), trusted_(false), phase_(Phase::IDLE), wanted_(Direction::NORTH), turnOnly_(false) {}

    void Navigator::reset(Position start, Direction heading) {
        pose_ = GridPose(start, heading);
        trusted_ = true;
        phase_ = Phase::IDLE;
    }

    bool Navigator::startStep(Direction dir) {
        if (phase_ != Phase::IDLE || !trusted_) return false;
        if (!maze_.canMove(pose_.p, dir)) return false;
        wanted_ = dir;
        turnOnly_ = false;
        phase_ = Phase::TURNING;
        motion_.startTurn(dir);
        return true;
    }

    bool Navigator::startTurn(Direction dir) {
        if (phase_ != Phase::IDLE || !trusted_) return false;
        wanted_ = dir;
        turnOnly_ = true;
        phase_ = Phase::TURNING;
        motion_.startTurn(dir);
        return true;
    }

    NavStatus Navigator::update() {
        if (phase_ == Phase::IDLE) return NavStatus::IDLE;

        const NavStatus s = motion_.update();
        if (s == NavStatus::RUNNING || s == NavStatus::IDLE) return NavStatus::RUNNING;

        if (phase_ == Phase::TURNING) {
            if (s == NavStatus::SUCCESS) {
                pose_.h = wanted_;
                if (turnOnly_) { phase_ = Phase::IDLE; return NavStatus::SUCCESS; }
                phase_ = Phase::MOVING;
                motion_.startMove(wanted_);
                return NavStatus::RUNNING;
            }
            trusted_ = false;
            phase_ = Phase::IDLE;
            return NavStatus::UNCERTAIN;
        }

        phase_ = Phase::IDLE;
        if (s == NavStatus::SUCCESS) {
            pose_.p = neighbor(pose_.p, wanted_);
            return NavStatus::SUCCESS;
        }
        if (s == NavStatus::UNCERTAIN) trusted_ = false;
        return s;
    }
}