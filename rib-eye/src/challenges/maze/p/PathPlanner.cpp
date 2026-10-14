#include "PathPlanner.hpp"

namespace maze {
    PlanResult PathPlanner::plan(const Maze& maze, Position from, Position to) {
        PlanResult result;
        if (!maze.isInside(from) || !maze.isInside(to)) return result;

        bool seen[kGridSize][kGridSize] = {};
        Position parent[kGridSize][kGridSize];
        Position queue[kGridSize * kGridSize];
        int head = 0, tail = 0;
        bool sawUnknown = false;

        queue[tail++] = from;
        seen[from.y][from.x] = true;

        while (head < tail) {
            const Position cur = queue[head++];
            if (cur == to) break;
            for (int i = 0; i < kDirCount; ++i) {
                const Direction d = kAll[i];
                const WallState c = maze.connection(cur, d);
                if (c == WallState::UNKNOWN) { sawUnknown = true; continue; }
                if (c != WallState::OPEN) continue;
                const Position n = neighbor(cur, d);
                if (!maze.isInside(n) || seen[n.y][n.x]) continue;
                seen[n.y][n.x] = true;
                parent[n.y][n.x] = cur;
                queue[tail++] = n;
            }
        }

        if (!seen[to.y][to.x]) {
            result.status = sawUnknown ? PlanStatus::BLOCKED_BY_UNKNOWN : PlanStatus::UNREACHABLE;
            return result;
        }

        std::vector<Position> rev;
        for (Position p = to; p != from; p = parent[p.y][p.x]) rev.push_back(p);
        rev.push_back(from);
        result.path.assign(rev.rbegin(), rev.rend());
        result.status = PlanStatus::FOUND;
        return result;
    }
}