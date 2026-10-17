#ifndef DEF_SECTION3
#define DEF_SECTION3

#include "LevelsStates.hpp"
#include "../../container/RobotContainer.hpp"

namespace levels {
    class Section3 {
        public:
            explicit Section3(RobotContainer& container);

            void begin();
            void update();
            void abort();

            bool done() const { return _state == Section3State::DONE; }
            bool failed() const { return _state == Section3State::FAULT; }
            Section3State state() const { return _state; }
            Section3Error error() const { return _error; }
            int tiles() const { return _tiles; }
            float targetYaw() const { return _targetYaw; }

        private:
            static constexpr int kColors = 8;

            RobotContainer& _container;
            Section3State _state;
            Section3Error _error;

            float _targetYaw;
            unsigned long _t0;
            int _votes[kColors];
            int _samples;
            uint8_t _retries;
            int _tiles;

            void fail(Section3Error e);
            void startAlign(float yawDeg);
            void startReading();
            void decide();
    };

}

#endif
