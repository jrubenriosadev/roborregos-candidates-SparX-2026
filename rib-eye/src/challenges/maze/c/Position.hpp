#pragma once
#include "Direction.hpp"

namespace maze {
    struct Position {
        int x;
        int y;
        constexpr Position() : x(0), y(0) {}
        constexpr Position(int _x, int _y) : x(_x), y(_y) {}
        constexpr bool operator==(const Position& o) const { return x == o.x && y == o.y; }
        constexpr bool operator!=(const Position& o) const { return x != o.x && y != o.y; }    
    };

    constexpr Position kInvalidPosition(-1,-1);
    constexpr Position neighbor(Position p, Direction d) {
        return d == Direction::NORTH ? Position(p.x, p.y-1) : Direction::EAST ? Position(p.x+1, p.y) : Direction::SOUTH ? Position(p.x, p.y+1) : Position(p.x-1,p.y);
    }

    constexpr bool isAdjacent(Position a, Position b) {
        return ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) == 1;
    }
}