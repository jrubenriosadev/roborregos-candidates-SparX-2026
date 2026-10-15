#include "MazeStateMachine.hpp"
#include "MazeConfig.hpp"
#include "../m/MapUpdater.hpp"
#include "../p/PathPlanner.hpp"

namespace maze {

    namespace {
        bool directionBetween(Position a, Position b, Direction& out) {
            for (int i = 0; i < kDirCount; ++i) {
                if (neighbor(a, kAllDirections[i]) == b) { out = kAllDirections[i]; return true; }
            }
            return false;
        }
    }

    const char* stateName(MazeState s) {
        switch (s) {
            case MazeState::INIT: return "INIT";
            case MazeState::EXPLORING: return "EXPLORING";
            case MazeState::GOING_TO_FINISH: return "GOING_TO_FINISH";
            case MazeState::RETURNING: return "RETURNING";
            case MazeState::DONE: return "DONE";
            case MazeState::FAULT: return "FAULT";
        }
        return "?";
    }

    const char* errorName(MazeError e) {
        switch (e) {
            case MazeError::NONE: return "NONE";
            case MazeError::HARDWARE_NOT_READY: return "HARDWARE_NOT_READY";
            case MazeError::SENSOR_INVALID: return "SENSOR_INVALID";
            case MazeError::MOVE_FAILED: return "MOVE_FAILED";
            case MazeError::POSE_UNCERTAIN: return "POSE_UNCERTAIN";
            case MazeError::DESYNC: return "DESYNC";
            case MazeError::EXPLORATION_INCOMPLETE: return "EXPLORATION_INCOMPLETE";
            case MazeError::FINISH_NOT_FOUND: return "FINISH_NOT_FOUND";
            case MazeError::NO_ROUTE_TO_FINISH: return "NO_ROUTE_TO_FINISH";
            case MazeError::TRAJECTORY_INVALID: return "TRAJECTORY_INVALID";
        }
        return "?";
    }

    MazeStateMachine::MazeStateMachine(Maze& maze, Sensors& sensors, MotionController& motion)
        : maze_(maze), 
          sensors_(sensors), 
          motion_(motion),
          navigator_(maze, motion), 
          explorer_(maze),
          trajectory_(), 
          returnLog_(),
          state_(MazeState::INIT), 
          phase_(Phase::START_SCAN), 
          error_(MazeError::NONE),
          scanRetries_(0), 
          moveRetries_(0), 
          initTicks_(0), 
          turnedForBack_(false),
          pending_(), 
          stepFrom_(), 
          stepDir_(Direction::NORTH), 
          routeIdx_(0) {}

    const char* MazeStateMachine::phaseName() const {
        switch (phase_) {
            case Phase::START_SCAN: return "START_SCAN";
            case Phase::SCANNING: return "SCANNING";
            case Phase::TURN_FOR_BACK: return "TURN_FOR_BACK";
            case Phase::DECIDE: return "DECIDE";
            case Phase::STEPPING: return "STEPPING";
            case Phase::ROUTE_NEXT: return "ROUTE_NEXT";
            case Phase::ROUTE_STEPPING: return "ROUTE_STEPPING";
        }
        return "?";
    }

    void MazeStateMachine::update() {
        switch (state_) {
            case MazeState::INIT: updateInit(); break;
            case MazeState::EXPLORING: updateExploring(); break;
            case MazeState::GOING_TO_FINISH:
            case MazeState::RETURNING: updateTravel(); break;
            case MazeState::DONE:
            case MazeState::FAULT: break;
        }
    }

    void MazeStateMachine::fail(MazeError e) {
        if (state_ == MazeState::FAULT) return;
        error_ = e;
        enter(MazeState::FAULT);
    }

    void MazeStateMachine::enter(MazeState next) {
        state_ = next;
        switch (next) {
            case MazeState::EXPLORING: phase_ = Phase::START_SCAN; break;
            case MazeState::GOING_TO_FINISH: prepareFinishRoute(); break;
            case MazeState::RETURNING: prepareReturnRoute(); break;
            case MazeState::DONE:
            case MazeState::FAULT: motion_.abort(); break;
            default: break;
        }
    }

    void MazeStateMachine::updateInit() {
        if (++initTicks_ > MazeConfig::kInitMaxTicks) { fail(MazeError::HARDWARE_NOT_READY); return; }
        if (!sensors_.ready() || !motion_.begin()) return;

        maze_.reset();
        const Position start = maze_.start();
        navigator_.reset(start, Direction::NORTH);
        trajectory_.initialize(start);
        returnLog_.clear();
        explorer_.begin(start);
        scanRetries_ = 0;
        moveRetries_ = 0;
        turnedForBack_ = false;
        enter(MazeState::EXPLORING);
    }

    bool MazeStateMachine::startCellBackUnknown() const {
        const GridPose& p = navigator_.pose();
        return !turnedForBack_ && trajectory_.size() == 1 && p.position == maze_.start() &&
            maze_.connection(p.position, opposite(p.heading)) == WallState::UNKNOWN;
    }

    bool MazeStateMachine::beginStep(Direction dir) {
        stepFrom_ = navigator_.pose().position;
        stepDir_ = dir;
        if (!navigator_.startStep(dir)) { fail(MazeError::DESYNC); return false; }
        return true;
    }

    void MazeStateMachine::onStepFailed(NavStatus s) {
        if (s == NavStatus::FAILED && moveRetries_ < MazeConfig::kMaxMoveRetries) {
            ++moveRetries_;
            beginStep(stepDir_);
            return;
        }
        fail(s == NavStatus::FAILED ? MazeError::MOVE_FAILED : MazeError::POSE_UNCERTAIN);
    }

    void MazeStateMachine::updateExploring() {
        switch (phase_) {
            case Phase::START_SCAN:
                sensors_.beginScan();
                phase_ = Phase::SCANNING;
                break;

            case Phase::SCANNING: {
                SensorReadings r;
                if (!sensors_.pollScan(r)) break;

                const GridPose p = navigator_.pose();
                const IntegrationResult res = integrateReadings(maze_, p, r);
                if (r.colorValid && r.color == MazeConfig::kFinishColor) maze_.setFinish(p.position);

                const bool wallsUnknown = res.unknownLeft > 0;
                const bool colorMissing = !r.colorValid && maze_.cell(p.position).color == CellColor::UNKNOWN;

                if (!wallsUnknown && !colorMissing) { scanRetries_ = 0; phase_ = Phase::DECIDE; break; }

                if (wallsUnknown && res.unknownLeft == 1 && startCellBackUnknown()) {
                    turnedForBack_ = true;
                    if (!navigator_.startTurn(turnAround(p.heading))) { fail(MazeError::DESYNC); break; }
                    phase_ = Phase::TURN_FOR_BACK;
                    break;
                }
                if (scanRetries_ < MazeConfig::kMaxScanRetries) { ++scanRetries_; phase_ = Phase::START_SCAN; break; }
                if (wallsUnknown) { fail(MazeError::SENSOR_INVALID); break; }
                scanRetries_ = 0;
                phase_ = Phase::DECIDE;
                break;
            }

            case Phase::TURN_FOR_BACK: {
                const NavStatus s = navigator_.update();
                if (s == NavStatus::RUNNING) break;
                if (s == NavStatus::SUCCESS) { scanRetries_ = 0; phase_ = Phase::START_SCAN; }
                else fail(MazeError::POSE_UNCERTAIN);
                break;
            }

            case Phase::DECIDE: {
                const ExploreDecision d = explorer_.decide(navigator_.pose());
                switch (d.type) {
                    case ExploreType::COMPLETE: enter(MazeState::GOING_TO_FINISH); break;
                    case ExploreType::INCOMPLETE: fail(MazeError::EXPLORATION_INCOMPLETE); break;
                    case ExploreType::DESYNC: fail(MazeError::DESYNC); break;
                    case ExploreType::MOVE_NEW:
                    case ExploreType::BACKTRACK:
                        pending_ = d;
                        moveRetries_ = 0;
                        if (beginStep(d.dir)) phase_ = Phase::STEPPING;
                        break;
                }
                break;
            }

            case Phase::STEPPING: {
                const NavStatus s = navigator_.update();
                if (s == NavStatus::RUNNING) break;
                if (s != NavStatus::SUCCESS) { onStepFailed(s); break; }

                const Position now = navigator_.pose().position;
                if (!trajectory_.recordSuccessfulMove(Move(stepFrom_, now, stepDir_))) { fail(MazeError::TRAJECTORY_INVALID); break; }
                if (now != pending_.target || !explorer_.commit(pending_)) { fail(MazeError::DESYNC); break; }
                moveRetries_ = 0;
                scanRetries_ = 0;
                phase_ = maze_.isVisited(now) ? Phase::DECIDE : Phase::START_SCAN;
                break;
            }

            default:
                fail(MazeError::DESYNC);
                break;
        }
    }

    void MazeStateMachine::prepareFinishRoute() {
        if (!maze_.hasFinish()) { fail(MazeError::FINISH_NOT_FOUND); return; }
        const PlanResult plan = PathPlanner::plan(maze_, navigator_.pose().position, maze_.finish());
        if (plan.status != PlanStatus::FOUND) { fail(MazeError::NO_ROUTE_TO_FINISH); return; }
        route_ = plan.path;
        routeIdx_ = 0;
        phase_ = Phase::ROUTE_NEXT;
    }

    void MazeStateMachine::prepareReturnRoute() {
        if (trajectory_.isEmpty() || trajectory_.first() != maze_.start() ||
            trajectory_.last() != navigator_.pose().position || !navigator_.poseTrusted()) {
            fail(MazeError::TRAJECTORY_INVALID);
            return;
        }
        route_ = trajectory_.reversedPath();
        routeIdx_ = 0;
        returnLog_.initialize(route_[0]);
        phase_ = Phase::ROUTE_NEXT;
    }

    void MazeStateMachine::onRouteDone() {
        if (state_ == MazeState::GOING_TO_FINISH) {
            if (navigator_.pose().position == maze_.finish()) enter(MazeState::RETURNING);
            else fail(MazeError::DESYNC);
            return;
        }
        if (navigator_.pose().position == maze_.start() && returnLog_.matchesReverseOf(trajectory_)) enter(MazeState::DONE);
        else fail(MazeError::TRAJECTORY_INVALID);
    }

    void MazeStateMachine::updateTravel() {
        switch (phase_) {
            case Phase::ROUTE_NEXT: {
                if (routeIdx_ + 1 >= route_.size()) { onRouteDone(); break; }
                const Position cur = route_[routeIdx_];
                const Position nxt = route_[routeIdx_ + 1];
                if (navigator_.pose().position != cur) { fail(MazeError::DESYNC); break; }
                Direction d = Direction::NORTH;
                if (!directionBetween(cur, nxt, d)) { fail(MazeError::TRAJECTORY_INVALID); break; }
                moveRetries_ = 0;
                if (beginStep(d)) phase_ = Phase::ROUTE_STEPPING;
                break;
            }

            case Phase::ROUTE_STEPPING: {
                const NavStatus s = navigator_.update();
                if (s == NavStatus::RUNNING) break;
                if (s != NavStatus::SUCCESS) { onStepFailed(s); break; }

                const Position now = navigator_.pose().position;
                if (now != route_[routeIdx_ + 1]) { fail(MazeError::DESYNC); break; }
                Trajectory& log = (state_ == MazeState::GOING_TO_FINISH) ? trajectory_ : returnLog_;
                if (!log.recordSuccessfulMove(Move(stepFrom_, now, stepDir_))) { fail(MazeError::TRAJECTORY_INVALID); break; }
                ++routeIdx_;
                phase_ = Phase::ROUTE_NEXT;
                break;
            }

            default:
                fail(MazeError::DESYNC);
                break;
        }
    }
}