#pragma once
#include "../c/CellColor.hpp"

namespace maze {
    namespace MazeConfig {

    constexpr CellColor kFinishColor = CellColor::RED;

    constexpr int kMaxScanRetries = 2;
    constexpr int kMaxMoveRetries = 2;
    constexpr int kInitMaxTicks   = 750;

    constexpr unsigned long kScanSettleMs   = 700;
    constexpr unsigned long kScanWindowMs   = 400;
    constexpr int kScanMinSamples = 10;
    constexpr int kColorAgreePct  = 60;
    constexpr unsigned long kTurnTimeoutMs  = 3500;
    constexpr unsigned long kTurnSettleMs   = 300;
    constexpr unsigned long kMoveTimeoutMs  = 4500;
    constexpr float kMinProgressFrac = 0.2f;

    }
}