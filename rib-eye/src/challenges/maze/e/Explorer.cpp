#include "Explorer.hpp"

namespace maze {

    constexpr std::size_t Explorer::kMaxDepth;

    Explorer::Explorer(const Maze& maze) : maze_(maze), size_(0) {}

    void Explorer::begin(Position start) {
        stack_[0] = start;
        size_ = 1;
    }

    ExploreDecision Explorer::decide(const GridPose& pose) const {
        ExploreDecision d;
        if (size_ == 0 || pose.p != stack_[size_ - 1]) { d.type = ExploreType::DESYNC; return d; }

        const Position cur = pose.p;
        if (maze_.unknownConnections(cur) > 0) { d.type = ExploreType::INCOMPLETE; return d; }

        static const Relative order[4] = {Relative::FRONT, Relative::LEFT, Relative::RIGHT, Relative::BACK};
        for (int i = 0; i < 4; ++i) {
            const Direction dir = toGlobal(pose.h, order[i]);
            if (!maze_.canMove(cur, dir)) continue;
            const Position n = neighbor(cur, dir);
            if (maze_.isVisited(n)) continue;
            d.type = ExploreType::MOVE_NEW;
            d.dir = dir;
            d.target = n;
            return d;
        }

        if (size_ > 1) {
            const Position parent = stack_[size_ - 2];
            for (int i = 0; i < kDirCount; ++i) {
                const Direction dir = kAll[i];
                if (neighbor(cur, dir) == parent && maze_.canMove(cur, dir)) {
                    d.type = ExploreType::BACKTRACK;
                    d.dir = dir;
                    d.target = parent;
                    return d;
                }
            }
            d.type = ExploreType::DESYNC;
            return d;
        }

        d.type = (maze_.visitedWithUnknownCount() > 0) ? ExploreType::INCOMPLETE : ExploreType::COMPLETE;
        return d;
    }

    bool Explorer::commit(const ExploreDecision& d) {
        if (d.type == ExploreType::MOVE_NEW) {
            if (size_ >= kMaxDepth) return false;
            stack_[size_++] = d.target;
            return true;
        }
        if (d.type == ExploreType::BACKTRACK) {
            if (size_ <= 1) return false;
            --size_;
            return true;
        }
        return false;
    }

}