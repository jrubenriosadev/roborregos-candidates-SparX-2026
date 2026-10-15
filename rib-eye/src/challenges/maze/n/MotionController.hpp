#pragma once
#include "../c/Direction.hpp"

namespace maze {

    enum class NavStatus : uint8_t {IDLE,RUNNING,SUCCESS,FAILED,UNCERTAIN};

    class MotionController {
        public:
            virtual ~MotionController() {}
            virtual bool begin() = 0;
            virtual void startTurn(Direction target) = 0;
            virtual void startMove(Direction heading) = 0;
            virtual NavStatus update() = 0;
            virtual void abort() = 0;
    };

}