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
            return maze.setConnection(pose.position, toGlobal(pose.heading, rel), wall ? WallState::WALL : WallState::OPEN);
        }
    }

    inline IntegrationResult integrateReadings(Maze& maze, const GridPose& pose, const SensorReadings& r) {
        IntegrationResult res;
        res.connectionsSet = 0;
        maze.markVisited(pose.position);
        if (detail::applyObservation(maze, pose, Relative::FRONT, r.frontWall, r.frontValid)) ++res.connectionsSet;
        if (detail::applyObservation(maze, pose, Relative::LEFT,  r.leftWall,  r.leftValid))  ++res.connectionsSet;
        if (detail::applyObservation(maze, pose, Relative::RIGHT, r.rightWall, r.rightValid)) ++res.connectionsSet;
        if (r.colorValid) maze.setColor(pose.position, r.color);
        res.unknownLeft = maze.unknownConnections(pose.position);
        return res;
    }
}