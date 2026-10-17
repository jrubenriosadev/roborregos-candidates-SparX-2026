#ifndef DEF_LEVELS
#define DEF_LEVELS

#include "LevelsStates.hpp"
#include "Section1.hpp"
#include "Section2.hpp"
#include "Section3.hpp"
#include "../maze/hardware/DriveMotion.hpp"
#include "../maze/hardware/RobotSensors.hpp"
#include "../maze/m/Maze.hpp"
#include "../../container/RobotContainer.hpp"

class Levels {
    public:
        explicit Levels(RobotContainer& container);

        void arm(levels::LevelsPhase first, ColorDetected startColor, bool runAll);
        void go();
        void abort();
        void update();

        levels::LevelsState state() const { return _state; }
        levels::LevelsPhase phase() const { return _phase; }
        const levels::Section1& section1() const { return _s1; }
        const levels::Section2& section2() const { return _s2; }
        const levels::Section3& section3() const { return _s3; }

    private:
        RobotContainer& _container;

        maze::Maze _maze;
        DriveMotion _motion;
        RobotSensors _sensors;
        levels::Section1 _s1;
        levels::Section2 _s2;
        levels::Section3 _s3;

        levels::LevelsState _state;
        levels::LevelsPhase _phase;
        ColorDetected _startColor;
        bool _runAll;
        bool _go;

        void startPhase(levels::LevelsPhase p);
        void nextPhase();
};

#endif
