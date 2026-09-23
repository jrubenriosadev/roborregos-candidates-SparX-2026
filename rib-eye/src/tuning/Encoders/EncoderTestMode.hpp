#ifndef DEF_ENCODERSTEST
#define DEF_ENCODERSTEST

#include "../../container/RobotContainer.hpp"

class EncoderTestMode {
    public:
        EncoderTestMode(RobotContainer& container);
        void run();
    private:
        RobotContainer& _container;
        unsigned long _lpt;
};

#endif