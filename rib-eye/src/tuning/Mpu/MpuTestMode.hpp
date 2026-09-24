#ifndef DEF_MPUTEST
#define DEF_MPUTEST

#include "../../container/RobotContainer.hpp"

class MpuTestMode {
    public:
        MpuTestMode(RobotContainer& container);
        void run();
    private:
        RobotContainer& _container;
        unsigned long _lpt;
};

#endif