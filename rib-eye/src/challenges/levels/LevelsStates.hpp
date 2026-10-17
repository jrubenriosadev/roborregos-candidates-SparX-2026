#pragma once

namespace levels {

    enum class Section1State { INIT, START_SCAN, SCANNING, TURN_FOR_BACK, DECIDE, TURNING, PEEKING, STEPPING, BALL_TURN, BALL_REVERSE, BALL_ADVANCE, ROUTE_NEXT, ROUTE_STEPPING, DONE, FAULT };
    enum class Section1Error { NONE, HARDWARE_NOT_READY, SENSOR_INVALID, MOVE_FAILED, POSE_UNCERTAIN, DESYNC, EXPLORATION_INCOMPLETE, FINISH_NOT_FOUND, NO_ROUTE_TO_FINISH };

    enum class Section2State { ALIGN, DRIVE, BACKOFF, TURN_SIDE, SHIFT, TURN_BACK, DONE, FAULT };
    enum class Section2Error { NONE, NO_FREE_LANE, MOVE_TIMEOUT, TURN_TIMEOUT };

    enum class Section3State { ALIGN, TURN_DELAY, MOVE, SETTLE, READING, DONE, FAULT };
    enum class Section3Error { NONE, COLOR_UNREADABLE, MOVE_TIMEOUT, TURN_TIMEOUT, TOO_MANY_TILES };

    enum class LevelsPhase { SECTION1, SECTION2, SECTION3 };
    enum class LevelsState { WAITING, SECTION1, SECTION2, SECTION3, DONE, FAULT };

    inline const char* stateName(Section1State s) {
        switch (s) {
            case Section1State::INIT: return "INIT";
            case Section1State::START_SCAN: return "START_SCAN";
            case Section1State::SCANNING: return "SCANNING";
            case Section1State::TURN_FOR_BACK: return "TURN_FOR_BACK";
            case Section1State::DECIDE: return "DECIDE";
            case Section1State::TURNING: return "TURNING";
            case Section1State::PEEKING: return "PEEKING";
            case Section1State::STEPPING: return "STEPPING";
            case Section1State::BALL_TURN: return "BALL_TURN";
            case Section1State::BALL_REVERSE: return "BALL_REVERSE";
            case Section1State::BALL_ADVANCE: return "BALL_ADVANCE";
            case Section1State::ROUTE_NEXT: return "ROUTE_NEXT";
            case Section1State::ROUTE_STEPPING: return "ROUTE_STEPPING";
            case Section1State::DONE: return "DONE";
            case Section1State::FAULT: return "FAULT";
        }
        return "?";
    }

    inline const char* errorName(Section1Error e) {
        switch (e) {
            case Section1Error::NONE: return "NONE";
            case Section1Error::HARDWARE_NOT_READY: return "HARDWARE_NOT_READY";
            case Section1Error::SENSOR_INVALID: return "SENSOR_INVALID";
            case Section1Error::MOVE_FAILED: return "MOVE_FAILED";
            case Section1Error::POSE_UNCERTAIN: return "POSE_UNCERTAIN";
            case Section1Error::DESYNC: return "DESYNC";
            case Section1Error::EXPLORATION_INCOMPLETE: return "EXPLORATION_INCOMPLETE";
            case Section1Error::FINISH_NOT_FOUND: return "FINISH_NOT_FOUND";
            case Section1Error::NO_ROUTE_TO_FINISH: return "NO_ROUTE_TO_FINISH";
        }
        return "?";
    }

    inline const char* stateName(Section2State s) {
        switch (s) {
            case Section2State::ALIGN: return "ALIGN";
            case Section2State::DRIVE: return "DRIVE";
            case Section2State::BACKOFF: return "BACKOFF";
            case Section2State::TURN_SIDE: return "TURN_SIDE";
            case Section2State::SHIFT: return "SHIFT";
            case Section2State::TURN_BACK: return "TURN_BACK";
            case Section2State::DONE: return "DONE";
            case Section2State::FAULT: return "FAULT";
        }
        return "?";
    }

    inline const char* errorName(Section2Error e) {
        switch (e) {
            case Section2Error::NONE: return "NONE";
            case Section2Error::NO_FREE_LANE: return "NO_FREE_LANE";
            case Section2Error::MOVE_TIMEOUT: return "MOVE_TIMEOUT";
            case Section2Error::TURN_TIMEOUT: return "TURN_TIMEOUT";
        }
        return "?";
    }

    inline const char* stateName(Section3State s) {
        switch (s) {
            case Section3State::ALIGN: return "ALIGN";
            case Section3State::TURN_DELAY: return "TURN_DELAY";
            case Section3State::MOVE: return "MOVE";
            case Section3State::SETTLE: return "SETTLE";
            case Section3State::READING: return "READING";
            case Section3State::DONE: return "DONE";
            case Section3State::FAULT: return "FAULT";
        }
        return "?";
    }

    inline const char* errorName(Section3Error e) {
        switch (e) {
            case Section3Error::NONE: return "NONE";
            case Section3Error::COLOR_UNREADABLE: return "COLOR_UNREADABLE";
            case Section3Error::MOVE_TIMEOUT: return "MOVE_TIMEOUT";
            case Section3Error::TURN_TIMEOUT: return "TURN_TIMEOUT";
            case Section3Error::TOO_MANY_TILES: return "TOO_MANY_TILES";
        }
        return "?";
    }

    inline const char* stateName(LevelsState s) {
        switch (s) {
            case LevelsState::WAITING: return "WAITING";
            case LevelsState::SECTION1: return "SECTION1";
            case LevelsState::SECTION2: return "SECTION2";
            case LevelsState::SECTION3: return "SECTION3";
            case LevelsState::DONE: return "DONE";
            case LevelsState::FAULT: return "FAULT";
        }
        return "?";
    }

}
