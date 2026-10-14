#pragma once
#include "Position.hpp"

namespace maze {
    struct GridPose {
        Position p;
        Direction h;
        GridPose() : p(), h(Direction::NORTH) {}
        GridPose(Position _p, Direction _h) : p(_p), h(_h) {}
    };
}