#ifndef DEF_STATEMACHINE
#define DEF_STATEMACHINE

#include "../container/RobotContainer.hpp"

enum class States {
    INIT,
    MOVE,
    IDLE
};

class StateMachine {
    public:
        StateMachine(RobotContainer& container);
        void update();

    private:
        RobotContainer& _container;
        States _currentState;
        unsigned long _pst;
};

#endif