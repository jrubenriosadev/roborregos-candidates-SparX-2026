#ifndef DEF_STATEMACHINE
#define DEF_STATEMACHINE

#include "../container/RobotContainer.hpp"

enum class States {
    WAIT_R,
    DELAY_BS,
    MOVE,
    TURN,
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
        ColorDetected _lastColor;

        const float SIDE_DISTANCE = 0.5f;
};

#endif