#pragma once
#include "Cell.hpp"
#include "../c/Position.hpp"

namespace maze {
    constexpr int kSize = 5;
    constexpr int kGridSize = 2*kSize - 1;
    constexpr Position kStartPosition(kSize-1, kSize-1);

    class Maze {
        private:
            Cell _cells[kGridSize][kGridSize];
            int _minX, _maxX, _minY, _maxY;
            Position _start;
            Position _end;
            bool _isEnd;
            int _conflicts;

            bool spanOk(Position p) const;
            void touch(Position p);
            Cell& mut(Position p) {return _cells[p.y][p.x];}

        public:
            Maze();
            void reset();

            bool inBuffer(Position p) const;
            bool isInside(Position p) const;
            const Cell& cell(Position p) const;

            WallState connection(Position p, Direction d) const;

            bool setConnection(Position p, Direction d, WallState s);
            bool canMove(Position p, Direction d) const;
            // Cierra todas las salidas de la casilla menos keepOpen (la pelota/el checkpoint son callejones aunque el sensor vea abierto)
            void seal(Position p, Direction keepOpen);

            void markVisited(Position p);
            bool isVisited(Position p) const;
            bool setColor(Position p, CellColor c);

            Position start() const {return _start;}
            void setEnd(Position p);
            bool hasEnded() const {return _isEnd;}
            Position end() const {return _end;}

            int unknownConnections(Position p) const;
            int visitedWithUnknownCount() const;
            int visitedCount() const;
            int conflictCount() const {return _conflicts;}
;    };
}