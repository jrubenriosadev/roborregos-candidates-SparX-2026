#pragma once
#include "../c/CellColor.hpp"

namespace maze {
    struct SensorReadings {
        bool fWall, lWall, rWall;
        bool fValid, lValid, rValid;
        CellColor color;
        bool cValid;

        SensorReadings() : fWall(false), lWall(false), rWall(false), fValid(false), lValid(false), rValid(false), color(CellColor::UNKNOWN), cValid(false) {}
    };
}