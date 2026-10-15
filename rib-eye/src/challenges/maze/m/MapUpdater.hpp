#pragma once
#include "Maze.hpp"
#include "../c/GridPose.hpp"
#include "../s/SensorReadings.hpp"

namespace maze {

    struct IntegrationResult {
        int connectionsSet;
        int unknownLeft;
    };

    namespace detail {
        inline bool applyObservation(Maze& maze, const GridPose& pose, Relative rel, bool wall, bool valid) {
            if (!valid) return false;
            return maze.setConnection(pose.p, toGlobal(pose.h, rel), wall ? WallState::WALL : WallState::OPEN);
        }
    }

    inline IntegrationResult integrateReadings(Maze& maze, const GridPose& pose, const SensorReadings& r) {
        IntegrationResult res;
        res.connectionsSet = 0;
        maze.markVisited(pose.p);
        if (detail::applyObservation(maze, pose, Relative::FRONT, r.fWall, r.fValid)) ++res.connectionsSet;
        if (detail::applyObservation(maze, pose, Relative::LEFT,  r.lWall,  r.lValid))  ++res.connectionsSet;
        if (detail::applyObservation(maze, pose, Relative::RIGHT, r.rWall, r.rValid)) ++res.connectionsSet;
        if (r.cValid) maze.setColor(pose.p, r.color);
        res.unknownLeft = maze.unknownConnections(pose.p);
        return res;
    }
}