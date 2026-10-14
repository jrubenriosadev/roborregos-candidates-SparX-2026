#pragma once
#include <vector>
#include "../m/Maze.hpp"

namespace maze {

    enum class PlanStatus { FOUND, UNREACHABLE, BLOCKED_BY_UNKNOWN, INVALID_ARGS};

    struct PlanResult {
        PlanStatus status;
        std::vector<Position> path;
        PlanResult() : status(PlanStatus::INVALID_ARGS) {}
    };

    class PathPlanner {
        public:
            static PlanResult plan(const Maze& maze, Position from, Position to);
    };
}