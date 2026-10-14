#pragma once
#include <cstddef>
#include <vector>
#include "../c/Position.hpp"

namespace maze {

    struct Move {
        Position from;
        Position to;
        Direction d;
        Move() : from(), to(), d(Direction::NORTH) {}
        Move(Position f, Position t, Direction d) : from(f), to(t), d(d) {}
    };

    class Trajectory {
        private:
            std::vector<Position> _path;
            std::vector<Move> _moves;
        public:
            void initialize(Position start);
            void clear();
            bool recordSuccessfulMove(const Move& m);

            const std::vector<Position>& getPath() const { return _path; }
            const std::vector<Move>& getMoves() const { return _moves; }
            bool isEmpty() const { return _path.empty(); }
            std::size_t size() const { return _path.size(); }
            Position first() const { return _path.front(); }
            Position last() const { return _path.back(); }

            std::vector<Position> reversedPath() const;
            bool matchesReverseOf(const Trajectory& original) const;
    };

}