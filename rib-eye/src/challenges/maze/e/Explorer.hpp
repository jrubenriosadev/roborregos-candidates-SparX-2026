#pragma once
#include <cstddef>
#include "../c/GridPose.hpp"
#include "../m/Maze.hpp"

namespace maze {

    enum class ExploreType {MOVE_NEW, BACKTRACK, COMPLETE, INCOMPLETE, DESYNC};

    struct ExploreDecision {
        ExploreType type;
        Direction dir;
        Position target;
        ExploreDecision() : type(ExploreType::DESYNC), dir(Direction::NORTH), target() {}
    };

    class Explorer {
        private:
            static constexpr std::size_t kMaxDepth = kSize * kSize + 1;
            const Maze& maze_;
            Position stack_[kMaxDepth];
            std::size_t size_;
        
        public:
            explicit Explorer(const Maze& maze);
            void begin(Position start);
            ExploreDecision decide(const GridPose& pose) const;
            bool commit(const ExploreDecision& d);
            std::size_t depth() const { return size_; }
    };
}