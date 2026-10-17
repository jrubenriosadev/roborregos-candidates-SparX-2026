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
            case ColorDetected::LIME:return maze::CellColor::LIME;
            case ColorDetected::NONE:return maze::CellColor::UNKNOWN;
        }
        return maze::CellColor::UNKNOWN;
    }
}

bool RobotSensors::ready() { return c_.tofReady(); }

unsigned RobotSensors::frontMm() { return c_.tofReady() ? c_.getTofLeftMm() : 2000; }

void RobotSensors::beginScan() {
    active_ = true;
    start_ = millis();
    n_ = front_ = left_ = right_ = 0;
    for (int i = 0; i < kColors; ++i) colorVotes_[i] = 0;
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
    if (el < maze::MazeConfig::kScanSettleMs) return false; 
    if (el < maze::MazeConfig::kScanSettleMs + maze::MazeConfig::kScanWindowMs) { sample(); return false; }

    active_ = false;
    out = maze::SensorReadings();
    const bool enough = n_ >= maze::MazeConfig::kScanMinSamples;
    out.fWall = front_ * 2 > n_;
    out.lWall = left_ * 2 > n_;
    out.rWall = right_ * 2 > n_;
    out.fValid = enough && c_.tofReady();
    out.lValid = enough;
    out.rValid = enough;

    int best = 0;
    for (int i = 1; i < kColors; ++i) if (colorVotes_[i] > colorVotes_[best]) best = i;
    if (enough && colorVotes_[best] * 100 >= n_ * maze::MazeConfig::kColorAgreePct) {
        out.color = toCellColor(static_cast<ColorDetected>(best));
        out.cValid = true;
    }
    return true;
}