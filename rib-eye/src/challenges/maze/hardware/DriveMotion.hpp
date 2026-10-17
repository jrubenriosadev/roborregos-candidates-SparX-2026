#pragma once
#include "../../../container/RobotContainer.hpp"
#include "../../levels/BallMotion.hpp"

class DriveMotion : public levels::BallMotion {
    public:
        explicit DriveMotion(RobotContainer& container) : c_(container) {}
        bool begin() override;
        void startTurn(maze::Direction target) override;
        void startMove(maze::Direction heading) override;
        maze::NavStatus update() override;
        void abort() override;
        void startReverse(maze::Direction heading, float meters) override;
        void startAdvance(maze::Direction heading, float meters) override;

    private:
        enum class Mode { IDLE, APPROACH, TURN, SETTLE, MOVE, REVERSE };
        RobotContainer& c_;
        float baseYaw_ = 0.0f;
        float turnYaw_ = 0.0f;
        bool haveBase_ = false;
        Mode mode_ = Mode::IDLE;
        unsigned long t0_ = 0;

        float yawFor(maze::Direction d) const;
};