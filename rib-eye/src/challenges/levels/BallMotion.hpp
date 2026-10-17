#pragma once
#include "../maze/n/MotionController.hpp"

namespace levels {
    class BallMotion : public maze::MotionController {
        public:
            virtual void startReverse(maze::Direction heading, float meters) = 0;
            virtual void startAdvance(maze::Direction heading, float meters) = 0;
    };

}
