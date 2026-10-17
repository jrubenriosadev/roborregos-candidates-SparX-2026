#pragma once
#include "SensorReadings.hpp"

namespace maze {
    class Sensors{
        public:
            virtual ~Sensors() {}
            virtual bool ready() = 0;
            virtual void beginScan() = 0;
            virtual bool pollScan(SensorReadings& out) = 0;
            // Lectura cruda del ToF frontal en mm (2000 = nada a la vista). Levels la usa para ver la pelota.
            virtual unsigned frontMm() { return 2000; }
    };

}