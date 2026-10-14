#include "Trajectory.hpp"

namespace maze {

void Trajectory::initialize(Position start) {
    _path.clear();
    _moves.clear();
    _path.reserve(64);
    _moves.reserve(64);
    _path.push_back(start);
}

void Trajectory::clear() {
    _path.clear();
    _moves.clear();
}

bool Trajectory::recordSuccessfulMove(const Move& m) {
    if (_path.empty()) return false;
    if (_path.back() != m.from) return false;
    if (neighbor(m.from, m.d) != m.to) return false;
    _path.push_back(m.to);
    _moves.push_back(m);
    return true;
}

std::vector<Position> Trajectory::reversedPath() const {
    return std::vector<Position>(_path.rbegin(), _path.rend());
}

bool Trajectory::matchesReverseOf(const Trajectory& original) const {
    if (_path.empty() || _path.size() != original._path.size()) return false;
    for (std::size_t i = 0; i < _path.size(); ++i)
        if (_path[i] != original._path[original._path.size() - 1 - i]) return false;
    return true;
}

}