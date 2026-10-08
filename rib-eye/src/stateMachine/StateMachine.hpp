#ifndef DEF_STATEMACHINE
#define DEF_STATEMACHINE

#include "../container/RobotContainer.hpp"

enum class States {
    WAIT_GREEN,
    PREP_TILE,
    MOVING_TILE,
    STOPPED
};

class StateMachine {
    public:
        StateMachine(RobotContainer& container);
        void update();

    private:
        RobotContainer& _container;
        States _currentState;
        const float TILE_SIZE_METERS = 0.30f;
        
        bool _redDetectedInTile; 
};

#endif