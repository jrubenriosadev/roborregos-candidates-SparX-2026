#pragma once
#include <cstddef>
#include <vector>
#include "BallMotion.hpp"
#include "LevelsStates.hpp"
#include "../maze/e/Explorer.hpp"
#include "../maze/m/Maze.hpp"
#include "../maze/n/Navigator.hpp"
#include "../maze/s/Sensors.hpp"

namespace levels {
    class Section1 {
        public:
            Section1(maze::Maze& maze, maze::Sensors& sensors, BallMotion& motion);

            void begin();
            void update();
            void abort();

            bool done() const { return state_ == Section1State::DONE; }
            bool failed() const { return state_ == Section1State::FAULT; }
            bool hasBall() const { return hasBall_; }
            Section1State state() const { return state_; }
            Section1Error error() const { return error_; }
            const maze::GridPose& pose() const { return navigator_.pose(); }

        private:
            maze::Maze& maze_;
            maze::Sensors& sensors_;
            BallMotion& motion_;
            maze::Navigator navigator_;
            maze::Explorer explorer_;

            Section1State state_;
            Section1Error error_;
            bool hasBall_;
            bool turnedForBack_;
            int scanRetries_;
            int moveRetries_;
            int initTicks_;
            int peekTicks_;
            int peekN_;
            int peekHits_;
            maze::ExploreDecision pending_;
            maze::Position stepFrom_;
            maze::Direction stepDir_;
            std::vector<maze::Position> route_;
            std::size_t routeIdx_;

            void fail(Section1Error e);
            void updateInit();
            void updateScanning();
            void updateDecide();
            void updatePeeking();
            void updateStepping();
            void updateRoute();
            bool beginStep(maze::Direction dir);
            void onStepFailed(maze::NavStatus s);
            void afterAligned();
            void startRoute();
            void finish();
            bool startCellBackUnknown() const;
    };

}
