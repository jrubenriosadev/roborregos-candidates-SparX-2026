#pragma once
#include <stdint.h>

namespace maze {
    enum class Direction : uint8_t {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};
    enum class Relative : uint8_t {FRONT = 0, RIGHT = 1, BACK = 2, LEFT = 3};

    constexpr int kDirCount = 4;
    constexpr Direction kAll[kDirCount] = {Direction::NORTH, Direction::EAST, Direction::SOUTH, Direction::WEST};

    constexpr int toIndex(Direction d) {return static_cast<int>(d);}
    constexpr Direction fromIndex(int i) {return static_cast<Direction>(((i%4)+4)%4);}
    constexpr Direction turnRight(Direction d) {return fromIndex(toIndex(d) + 1);}
    constexpr Direction turnLeft(Direction d) {return fromIndex(toIndex(d) + 3);}
    constexpr Direction turn180(Direction d) {return fromIndex(toIndex(d) + 2);}
    constexpr Direction opposite(Direction d) {return turn180(d);}

    constexpr Direction toGlobal(Direction h, Relative r) {return fromIndex(toIndex(h)+ static_cast<int>(rel));}
    constexpr Relative toRelative(Direction h, Direction global) {return static_cast<Relative>(((toIndex(global) - toIndex(h))%4+4)%4);}
    constexpr int quarterTurnsCW(Direction from, Direction to) {return ((toIndex(to) - toIndex(from))%4+4)%4;}
    
    inline const char* d_name(Direction d) {
        switch(d) {
            case Direction::NORTH: return "N";
            case Direction::EAST return "E";
            case Direction::SOUTH return "S";
            case Direction::WEST return "W";
        }
        return "X";
    }
}