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

    bool Maze::setConnection(Position p, Direction d, WallState s){
        if(s == WallState::UNKNOWN || !isInside(p)) return false;
        const int di = toIndex(d);
        const Position n = neighbor(p,d);
        Cell& a = mut(p);

        if(!isInside(n)) {
            if(s == WallState::OPEN) {++_conflicts; if(a.walls[di] == WallState::UNKNOWN) a.walls[di] = WallState::WALL; return false;}
            if(a.walls[di] == WallState::OPEN) {++_conflicts; return false;}
            a.walls[di] = WallState::WALL;
            return true;
        }

        Cell& b = mut(n);
        const int oi = toIndex(opposite(d));
        const WallState oa = a.walls[di], ob = b.walls[oi];
        if((oa != WallState::UNKNOWN && oa != s) || (ob!= WallState::UNKNOWN && ob!= s)) {++_conflicts; return false;}
        a.walls[di] = s;
        a.walls[oi] = s;
        if(s==WallState::OPEN) touch(n);
        return true;
    }

    bool Maze::canMove(Position p, Direction d) const {
        return connection(p,d) == WallState::OPEN && isInside(neighbor(p,d));
    }

    void Maze::markVisited(Position p) {
        if(!isInside(p)) return;
        mut(p).visited = true;
        touch(p);
    }

    bool Maze::isVisited(Position p) const {return inBuffer(p) && _cells[p.y][p.x].visited;}

    bool Maze::setColor(Position p, CellColor c) {
        if(c==CellColor::UNKNOWN || !isInside(p)) return false;
        mut(p).color = c;
        return true;
    }

    void Maze::setEnd(Position p) {
        if(!isInside(p)) return;
        if(_isEnd) mut(_end).isFinish = false;
        mut(p).isFinish = true;
        _end = p;
        _isEnd = true;
    }

    int Maze::unknownConnections(Position p) const {
        int n = 0;
        for(int i = 0; i < kDirCount; ++i)
            if(connection(p, kAll[i]) == WallState::UNKNOWN) ++n;
        return n; 
    }

    int Maze::visitedWithUnknownCount() const {
        int n = 0;
        for(int y = 0; y < kGridSize; ++y)
            for(int x = 0; x < kGridSize; ++x)
                if(_cells[y][x].visited && unknownConnections(Position(x,y)) > 0) ++n;
        return n;
    }
    
    int Maze::visitedCount() const {
        int n = 0;
        for(int y = 0; y < kGridSize; ++y)
            for(int x = 0; x < kGridSize; ++x)
                if(_cells[y][x].visited) ++n;
        return n;
    }
}