#pragma once
#include "MotionController.hpp"
#include "../c/GridPose.hpp"
#include "../m/Maze.hpp"

namespace maze {
    class Navigator {
        public:
            Navigator(const Maze& maze, MotionController& motion);

            void reset(Position start, Direction heading);
            bool startStep(Direction dir);
            bool startTurn(Direction dir);
            NavStatus update();

            const GridPose& pose() const { return pose_; }
            bool poseTrusted() const { return trusted_; }

        private:
            enum class Phase { IDLE, TURNING, MOVING };
            const Maze& maze_;
            MotionController& motion_;
            GridPose pose_;
            bool trusted_;
            Phase phase_;
            Direction wanted_;
            bool turnOnly_;
    };

}