#ifndef DEF_SECTION2
#define DEF_SECTION2

#include "LevelsStates.hpp"
#include "../../container/RobotContainer.hpp"

namespace levels {

    class Section2 {
        public:
            explicit Section2(RobotContainer& container);

            void begin();
            void update();
            void abort();

            bool done() const { return _state == Section2State::DONE; }
            bool failed() const { return _state == Section2State::FAULT; }
            Section2State state() const { return _state; }
            Section2Error error() const { return _error; }
            float progress() const { return _progress; }
            int lane() const { return _lane; }

        private:
            static constexpr int kNoLane = 99;

            RobotContainer& _container;
            Section2State _state;
            Section2Error _error;

            int _lane;
            int _target;
            uint8_t _tried;
            float _progress;
            float _segBase;
            float _blockAt;
            float _sideYaw;
            bool _recentering;
            bool _armed;
            unsigned long _t0;

            void fail(Section2Error e);
            bool lineBlocked() const;
            int nextLane() const;
            void startDrive();
            void startSideTurn();
    };

}

#endif
