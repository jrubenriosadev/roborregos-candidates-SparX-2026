#pragma once
#include "../../../container/RobotContainer.hpp"
#include "../s/Sensors.hpp"

class RobotSensors : public maze::Sensors {
    public:
        explicit RobotSensors(RobotContainer& container) : c_(container) {}
        bool ready() override;
        void beginScan() override;
        bool pollScan(maze::SensorReadings& out) override;

    private:
        RobotContainer& c_;
        bool active_ = false;
        unsigned long start_ = 0;
        int n_ = 0, front_ = 0, left_ = 0, right_ = 0;
        int colorVotes_[7] = {0, 0, 0, 0, 0, 0, 0};

        void sample();
};