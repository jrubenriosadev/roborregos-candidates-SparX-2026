#ifndef DEF_STATEMACHINE
#define DEF_STATEMACHINE

#include "../container/RobotContainer.hpp"

enum class States {
    INIT,
    A_MOVE,
    A_IDLE,
    B_TURN,
    B_IDLE
};

class StateMachine {
    public:
        StateMachine(RobotContainer& container);
        void update();
    private:
        RobotContainer& _container;
        States _currentState;
        MpuData _imuData;

        unsigned long _pst;
        bool _isLeft;
};

#endif