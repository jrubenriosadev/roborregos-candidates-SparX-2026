#include "RobotSensors.hpp"
#include "../comp/MazeConfig.hpp"

namespace {
    maze::CellColor toCellColor(ColorDetected c) {
        switch (c) {
            case ColorDetected::RED:return maze::CellColor::RED;
            case ColorDetected::GREEN:return maze::CellColor::GREEN;
            case ColorDetected::CYAN:return maze::CellColor::CYAN;
            case ColorDetected::YELLOW:return maze::CellColor::YELLOW;
            case ColorDetected::ORANGE:return maze::CellColor::ORANGE;
            case ColorDetected::MAGENTA:return maze::CellColor::MAGENTA;
            case ColorDetected::NONE:return maze::CellColor::OTHER;
        }
        return maze::CellColor::UNKNOWN;
    }
}

bool RobotSensors::ready() { return c_.tofReady(); }

void RobotSensors::beginScan() {
    active_ = true;
    start_ = millis();
    n_ = front_ = left_ = right_ = 0;
    for (int i = 0; i < 7; ++i) colorVotes_[i] = 0;
}

void RobotSensors::sample() {
    ++n_;
    if (c_.frontWall()) ++front_;
    if (c_.leftWall()) ++left_;
    if (c_.rightWall()) ++right_;
    ++colorVotes_[static_cast<int>(c_.getColorDetected())];
}

bool RobotSensors::pollScan(maze::SensorReadings& out) {
    if (!active_) return false;
    const unsigned long el = millis() - start_;
    if (el < maze::MazeConfig::kScanSettleMs) return false;                       // robot en reposo
    if (el < maze::MazeConfig::kScanSettleMs + maze::MazeConfig::kScanWindowMs) { sample(); return false; }

    active_ = false;
    out = maze::SensorReadings();
    const bool enough = n_ >= maze::MazeConfig::kScanMinSamples;
    out.frontWall = front_ * 2 > n_;
    out.leftWall = left_ * 2 > n_;
    out.rightWall = right_ * 2 > n_;
    out.frontValid = enough && c_.tofReady();
    out.leftValid = enough;
    out.rightValid = enough;

    int best = 0;
    for (int i = 1; i < 7; ++i) if (colorVotes_[i] > colorVotes_[best]) best = i;
    if (enough && colorVotes_[best] * 100 >= n_ * maze::MazeConfig::kColorAgreePct) {
        out.color = toCellColor(static_cast<ColorDetected>(best));
        out.colorValid = true;
    }
    return true;
}