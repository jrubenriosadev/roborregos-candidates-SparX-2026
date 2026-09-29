#ifndef DEF_STATEMACHINE
#define DEF_STATEMACHINE

#include "../container/RobotContainer.hpp"

enum class States {
    INIT,
    MOVE,
    PBT,
    TURN,
    IDLE,
    END
};

class StateMachine {
    public:
        StateMachine(RobotContainer& container);
        void update();

    private:
        RobotContainer& _container;
        States _currentState;

        unsigned long _pst;
        float _targetAngle;
        int _sideCount;

        const float SIDE_DISTANCE = 0.5f;
};

#endif