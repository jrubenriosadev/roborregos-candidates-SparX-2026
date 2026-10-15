#pragma once
#include "DriveMotion.hpp"
#include "RobotSensors.hpp"
#include "../comp/MazeStateMachine.hpp"

class MazeMain {
    public:
        explicit MazeMain(RobotContainer& container)
            : maze_(), motion_(container), sensors_(container), sm_(maze_, sensors_, motion_) {}

        void update() { sm_.update(); }
        const maze::MazeStateMachine& machine() const { return sm_; }

    private:
        maze::Maze maze_;
        DriveMotion motion_;
        RobotSensors sensors_;
        maze::MazeStateMachine sm_;
};