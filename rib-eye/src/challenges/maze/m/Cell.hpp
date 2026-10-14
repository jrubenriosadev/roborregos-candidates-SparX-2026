#pragma once
#include "../c/CellColor.hpp"
#include "../c/Direction.hpp"

namespace maze {
    enum class WallState : uint8_t {UNKNOWN, OPEN, WALL};

    struct Cell {
        bool visited;
        WallState walls[kDirCount];
        CellColor color;
        bool isStart;
        bool isFinish;

        Cell() : visited(false), color(CellColor::UNKNOWN), isStart(false), isFinish(false) {
            for(int i = 0; i<kDirCount; ++i) walls[i] = WallState::UNKNOWN;
        }
        WallState wall(Direction d) const { return walls[toIndex(d)];}
    };
}