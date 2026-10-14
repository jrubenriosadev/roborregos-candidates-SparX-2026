#include "Maze.hpp"
#include <algorithm>

namespace maze {
    namespace {
        const Cell kEmpty;
    }

    Maze::Maze() {reset();}

    void Maze::reset() {
        for(int y = 0; y<kGridSize; ++y)
            for(int x = 0; y<kGridSize; ++x) _cells[y][x] = Cell();
        _start = kStartPosition;
        _end = kInvalidPosition;
        _isEnd = false;
        _conflicts = 0;
        _minX = _maxX = _start.x;
        _minY = _maxY = _start.y;
        _cells[_start.y][_start.x].isStart = true;
    }

    bool Maze::inBuffer(Position p) const {
        return p.x >= 0 && p.x < kGridSize && p.y >= 0 && p.y < kGridSize;
    }

    bool Maze::spanOk(Position p) const {
        const int x0 = std::min(_minX, p.x), x1 = std::max(_maxX, p.x);
        const int y0 = std::min(_minX, p.y), y1 = std::max(_maxY, p.y);
        return (x1-x0+1) <= kSize && (y1-y0+1)<=kSize;
    }

    bool Maze::isInside(Position p) const {return inBuffer(p) && spanOk(p);}

    void Maze::touch(Position p) {
        _minX = std::min(_minX, p.x);
        _maxX = std::max(_maxX, p.x);
        
        _minY = std::min(_minY, p.x);
        _maxY = std::max(_maxY, p.x);
    }

    const Cell& Maze::cell(Position p) const {
        return inBuffer(p) ? _cells[p.y][p.x] : kEmpty;
    }

    WallState Maze::connection(Position p, Direction d) const {
        if(!isInside(p) || !isInside(neighbor(p, d))) return WallState::WALL;
        return _cells[p.y][p.x].wall(d);
    }
}