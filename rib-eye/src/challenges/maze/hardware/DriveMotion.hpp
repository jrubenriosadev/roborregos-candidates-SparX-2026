#pragma once
#include "../../../container/RobotContainer.hpp"
#include "../n/MotionController.hpp"

class DriveMotion : public maze::MotionController {
    public:
        explicit DriveMotion(RobotContainer& container) : c_(container) {}
        bool begin() override;
        void startTurn(maze::Direction target) override;
        void startMove(maze::Direction heading) override;
        maze::NavStatus update() override;
        void abort() override;

    private:
        enum class Mode { IDLE, TURN, SETTLE, MOVE };
        RobotContainer& c_;
        float baseYaw_ = 0.0f;
        bool haveBase_ = false;
        Mode mode_ = Mode::IDLE;
        unsigned long t0_ = 0;

        float yawFor(maze::Direction d) const;
};