#include "Section1.hpp"
#include "../maze/comp/MazeConfig.hpp"
#include "../maze/m/MapUpdater.hpp"
#include "../maze/p/PathPlanner.hpp"
#include "constants/levelsConfig.h"

namespace levels {

    namespace Cfg = LevelsConfig;

    namespace {
        bool directionBetween(maze::Position a, maze::Position b, maze::Direction& out) {
            for (int i = 0; i < maze::kDirCount; ++i) {
                if (maze::neighbor(a, maze::kAll[i]) == b) { out = maze::kAll[i]; return true; }
            }
            return false;
        }
    }

    Section1::Section1(maze::Maze& maze, maze::Sensors& sensors, BallMotion& motion)
        : maze_(maze),
          sensors_(sensors),
          motion_(motion),
          navigator_(maze, motion),
          explorer_(maze),
          state_(Section1State::INIT),
          error_(Section1Error::NONE),
          hasBall_(false),
          turnedForBack_(false),
          scanRetries_(0),
          moveRetries_(0),
          initTicks_(0),
          peekTicks_(0),
          peekN_(0),
          peekHits_(0),
          pending_(),
          stepFrom_(),
          stepDir_(maze::Direction::NORTH),
          routeIdx_(0) {}

    void Section1::begin() {
        state_ = Section1State::INIT;
        error_ = Section1Error::NONE;
        hasBall_ = false;
        initTicks_ = 0;
    }

    void Section1::abort() {
        motion_.abort();
        state_ = Section1State::FAULT;
    }

    void Section1::fail(Section1Error e) {
        if (state_ == Section1State::FAULT) return;
        error_ = e;
        motion_.abort();
        state_ = Section1State::FAULT;
    }

    void Section1::finish() {
        motion_.abort();
        state_ = Section1State::DONE;
    }

    void Section1::update() {
        switch (state_) {
            case Section1State::INIT: updateInit(); break;

            case Section1State::START_SCAN:
                sensors_.beginScan();
                state_ = Section1State::SCANNING;
                break;

            case Section1State::SCANNING: updateScanning(); break;

            case Section1State::TURN_FOR_BACK: {
                const maze::NavStatus s = navigator_.update();
                if (s == maze::NavStatus::RUNNING) break;
                if (s == maze::NavStatus::SUCCESS) { scanRetries_ = 0; state_ = Section1State::START_SCAN; }
                else fail(Section1Error::POSE_UNCERTAIN);
                break;
            }

            case Section1State::DECIDE: updateDecide(); break;

            case Section1State::TURNING: {
                const maze::NavStatus s = navigator_.update();
                if (s == maze::NavStatus::RUNNING) break;
                if (s == maze::NavStatus::SUCCESS) afterAligned();
                else fail(Section1Error::POSE_UNCERTAIN);
                break;
            }

            case Section1State::PEEKING: updatePeeking(); break;
            case Section1State::STEPPING: updateStepping(); break;

            case Section1State::BALL_TURN: {
                const maze::NavStatus s = navigator_.update();
                if (s == maze::NavStatus::RUNNING) break;
                if (s != maze::NavStatus::SUCCESS) { fail(Section1Error::POSE_UNCERTAIN); break; }
                motion_.startReverse(navigator_.pose().h, Cfg::kBallReverseM);
                state_ = Section1State::BALL_REVERSE;
                break;
            }

            case Section1State::BALL_REVERSE: {
                const maze::NavStatus s = motion_.update();
                if (s == maze::NavStatus::RUNNING || s == maze::NavStatus::IDLE) break;
                if (s != maze::NavStatus::SUCCESS) { fail(Section1Error::MOVE_FAILED); break; }
                motion_.startAdvance(navigator_.pose().h, Cfg::kBallReverseM);
                state_ = Section1State::BALL_ADVANCE;
                break;
            }

            case Section1State::BALL_ADVANCE: {
                const maze::NavStatus s = motion_.update();
                if (s == maze::NavStatus::RUNNING || s == maze::NavStatus::IDLE) break;
                if (s != maze::NavStatus::SUCCESS) { fail(Section1Error::POSE_UNCERTAIN); break; }

                // Ya tenemos la pelota: la casilla donde estaba es un callejon (3 paredes), no se vuelve a entrar
                hasBall_ = true;
                maze_.markVisited(pending_.target);
                maze_.seal(pending_.target, maze::opposite(pending_.dir));
                state_ = Section1State::DECIDE;
                break;
            }

            case Section1State::ROUTE_NEXT:
            case Section1State::ROUTE_STEPPING: updateRoute(); break;

            case Section1State::DONE:
            case Section1State::FAULT: break;
        }
    }

    void Section1::updateInit() {
        if (++initTicks_ > maze::MazeConfig::kInitMaxTicks) { fail(Section1Error::HARDWARE_NOT_READY); return; }
        if (!sensors_.ready() || !motion_.begin()) return;

        maze_.reset();
        const maze::Position start = maze_.start();
        navigator_.reset(start, maze::Direction::NORTH);
        explorer_.begin(start);
        scanRetries_ = 0;
        moveRetries_ = 0;
        turnedForBack_ = false;
        state_ = Section1State::START_SCAN;
    }

    bool Section1::startCellBackUnknown() const {
        const maze::GridPose& p = navigator_.pose();
        return !turnedForBack_ && explorer_.depth() == 1 && p.p == maze_.start() &&
            maze_.connection(p.p, maze::opposite(p.h)) == maze::WallState::UNKNOWN;
    }

    void Section1::updateScanning() {
        maze::SensorReadings r;
        if (!sensors_.pollScan(r)) return;

        const maze::GridPose p = navigator_.pose();
        maze::integrateReadings(maze_, p, r);

        if (r.cValid && r.color == maze::CellColor::RED) {
            // El checkpoint es la frontera de la seccion: nunca se explora mas alla de el
            maze_.setEnd(p.p);
            maze_.seal(p.p, maze::opposite(p.h));
            if (hasBall_) { finish(); return; }
        }

        const int unknownLeft = maze_.unknownConnections(p.p);
        const bool wallsUnknown = unknownLeft > 0;
        const bool colorMissing = !r.cValid && maze_.cell(p.p).color == maze::CellColor::UNKNOWN;

        if (!wallsUnknown && !colorMissing) { scanRetries_ = 0; state_ = Section1State::DECIDE; return; }

        if (wallsUnknown && unknownLeft == 1 && startCellBackUnknown()) {
            turnedForBack_ = true;
            if (!navigator_.startTurn(maze::turn180(p.h))) { fail(Section1Error::DESYNC); return; }
            state_ = Section1State::TURN_FOR_BACK;
            return;
        }
        if (scanRetries_ < maze::MazeConfig::kMaxScanRetries) { ++scanRetries_; state_ = Section1State::START_SCAN; return; }
        if (wallsUnknown) { fail(Section1Error::SENSOR_INVALID); return; }
        scanRetries_ = 0;
        state_ = Section1State::DECIDE;
    }

    void Section1::updateDecide() {
        // Con pelota y checkpoint conocido ya no hay nada que explorar
        if (hasBall_ && maze_.hasEnded()) { startRoute(); return; }

        const maze::GridPose p = navigator_.pose();
        const maze::ExploreDecision d = explorer_.decide(p);
        switch (d.type) {
            case maze::ExploreType::COMPLETE:
                // Se exploro todo: sin pelota se va al checkpoint de todos modos (puntos parciales)
                if (maze_.hasEnded()) startRoute();
                else fail(Section1Error::FINISH_NOT_FOUND);
                break;
            case maze::ExploreType::INCOMPLETE: fail(Section1Error::EXPLORATION_INCOMPLETE); break;
            case maze::ExploreType::DESYNC: fail(Section1Error::DESYNC); break;
            case maze::ExploreType::MOVE_NEW:
            case maze::ExploreType::BACKTRACK:
                pending_ = d;
                moveRetries_ = 0;
                if (d.dir != p.h) {
                    if (!navigator_.startTurn(d.dir)) { fail(Section1Error::DESYNC); break; }
                    state_ = Section1State::TURNING;
                } else {
                    afterAligned();
                }
                break;
        }
    }

    void Section1::afterAligned() {
        // Antes de entrar a una casilla nueva se mira de frente: si ahi esta la pelota NO se entra de frente (se empujaria)
        if (pending_.type == maze::ExploreType::MOVE_NEW && !hasBall_) {
            peekTicks_ = 0;
            peekN_ = 0;
            peekHits_ = 0;
            state_ = Section1State::PEEKING;
            return;
        }
        if (beginStep(pending_.dir)) state_ = Section1State::STEPPING;
    }

    void Section1::updatePeeking() {
        ++peekTicks_;
        if (peekTicks_ <= Cfg::kPeekSettleTicks) return;

        if (peekTicks_ <= Cfg::kPeekSettleTicks + Cfg::kPeekWindowTicks) {
            const unsigned mm = sensors_.frontMm();
            ++peekN_;
            if (mm >= Cfg::kBallMinMm && mm <= Cfg::kBallMaxMm) ++peekHits_;
            return;
        }

        const bool ball = peekN_ >= Cfg::kPeekMinSamples && peekHits_ * 100 >= peekN_ * Cfg::kBallAgreePct;
        if (ball) {
            // El intake esta ATRAS: se gira 180 para meter la espalda a la casilla de la pelota
            if (!navigator_.startTurn(maze::turn180(navigator_.pose().h))) { fail(Section1Error::DESYNC); return; }
            state_ = Section1State::BALL_TURN;
            return;
        }
        if (beginStep(pending_.dir)) state_ = Section1State::STEPPING;
    }

    bool Section1::beginStep(maze::Direction dir) {
        stepFrom_ = navigator_.pose().p;
        stepDir_ = dir;
        if (!navigator_.startStep(dir)) { fail(Section1Error::DESYNC); return false; }
        return true;
    }

    void Section1::onStepFailed(maze::NavStatus s) {
        if (s == maze::NavStatus::FAILED && moveRetries_ < maze::MazeConfig::kMaxMoveRetries) {
            ++moveRetries_;
            if (beginStep(stepDir_)) state_ = (state_ == Section1State::ROUTE_STEPPING) ? Section1State::ROUTE_STEPPING : Section1State::STEPPING;
            return;
        }
        fail(s == maze::NavStatus::FAILED ? Section1Error::MOVE_FAILED : Section1Error::POSE_UNCERTAIN);
    }

    void Section1::updateStepping() {
        const maze::NavStatus s = navigator_.update();
        if (s == maze::NavStatus::RUNNING) return;
        if (s != maze::NavStatus::SUCCESS) { onStepFailed(s); return; }

        const maze::Position now = navigator_.pose().p;
        if (now != pending_.target || !explorer_.commit(pending_)) { fail(Section1Error::DESYNC); return; }
        moveRetries_ = 0;
        scanRetries_ = 0;
        state_ = maze_.isVisited(now) ? Section1State::DECIDE : Section1State::START_SCAN;
    }

    void Section1::startRoute() {
        const maze::PlanResult plan = maze::PathPlanner::plan(maze_, navigator_.pose().p, maze_.end());
        if (plan.status != maze::PlanStatus::FOUND) { fail(Section1Error::NO_ROUTE_TO_FINISH); return; }
        route_ = plan.path;
        routeIdx_ = 0;
        state_ = Section1State::ROUTE_NEXT;
    }

    void Section1::updateRoute() {
        if (state_ == Section1State::ROUTE_NEXT) {
            if (routeIdx_ + 1 >= route_.size()) {
                if (navigator_.pose().p == maze_.end()) finish();
                else fail(Section1Error::DESYNC);
                return;
            }
            const maze::Position cur = route_[routeIdx_];
            const maze::Position nxt = route_[routeIdx_ + 1];
            if (navigator_.pose().p != cur) { fail(Section1Error::DESYNC); return; }
            maze::Direction d = maze::Direction::NORTH;
            if (!directionBetween(cur, nxt, d)) { fail(Section1Error::DESYNC); return; }
            moveRetries_ = 0;
            if (beginStep(d)) state_ = Section1State::ROUTE_STEPPING;
            return;
        }

        const maze::NavStatus s = navigator_.update();
        if (s == maze::NavStatus::RUNNING) return;
        if (s != maze::NavStatus::SUCCESS) { onStepFailed(s); return; }
        if (navigator_.pose().p != route_[routeIdx_ + 1]) { fail(Section1Error::DESYNC); return; }
        ++routeIdx_;
        state_ = Section1State::ROUTE_NEXT;
    }

}
