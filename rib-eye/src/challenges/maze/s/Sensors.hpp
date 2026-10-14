#pragma once
#include "SensorReadings.hpp"

namespace maze {
    class Sensors{
        public:
            virtual ~Sensors() {}
            virtual bool ready() = 0;
            virtual void beginScan() = 0;
            virtual bool pollScan(SensorReadings& out) = 0;
    };

}