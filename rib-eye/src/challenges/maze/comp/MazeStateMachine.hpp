#pragma once
#include <cstddef>
#include <vector>
#include "../e/Explorer.hpp"
#include "../m/Maze.hpp"
#include "../n/Navigator.hpp"
#include "../s/Sensors.hpp"
#include "../t/Trajectory.hpp"

namespace maze {

    enum class MazeState { INIT, EXPLORING, GOING_TO_FINISH, RETURNING, DONE, FAULT };

    enum class MazeError {NONE, HARDWARE_NOT_READY, SENSOR_INVALID, MOVE_FAILED, POSE_UNCERTAIN, DESYNC, EXPLORATION_INCOMPLETE, FINISH_NOT_FOUND, NO_ROUTE_TO_FINISH, TRAJECTORY_INVALID};

    const char* stateName(MazeState s);
    const char* errorName(MazeError e);

    class MazeStateMachine {
    public:
        MazeStateMachine(Maze& maze, Sensors& sensors, MotionController& motion);

        void update();

        MazeState state() const { return state_; }
        MazeError error() const { return error_; }
        const char* phaseName() const;
        const GridPose& pose() const { return navigator_.pose(); }
        const Trajectory& trajectory() const { return trajectory_; }
        const Trajectory& returnLog() const { return returnLog_; }
        const Maze& map() const { return maze_; }
        bool finishFound() const { return maze_.hasFinish(); }

    private:
        enum class Phase { START_SCAN, SCANNING, TURN_FOR_BACK, DECIDE, STEPPING, ROUTE_NEXT, ROUTE_STEPPING };

        Maze& maze_;
        Sensors& sensors_;
        MotionController& motion_;
        Navigator navigator_;
        Explorer explorer_;
        Trajectory trajectory_;
        Trajectory returnLog_;

        MazeState state_;
        Phase phase_;
        MazeError error_;
        int scanRetries_;
        int moveRetries_;
        int initTicks_;
        bool turnedForBack_;
        ExploreDecision pending_;
        Position stepFrom_;
        Direction stepDir_;
        std::vector<Position> route_;
        std::size_t routeIdx_;

        void enter(MazeState next);
        void fail(MazeError e);
        void updateInit();
        void updateExploring();
        void updateTravel();
        void prepareFinishRoute();
        void prepareReturnRoute();
        void onRouteDone();
        bool beginStep(Direction dir);
        void onStepFailed(NavStatus s);
        bool startCellBackUnknown() const;
    };

}