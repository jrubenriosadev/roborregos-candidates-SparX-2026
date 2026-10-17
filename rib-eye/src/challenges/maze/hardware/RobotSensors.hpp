#pragma once
#include "../../../container/RobotContainer.hpp"
#include "../s/Sensors.hpp"

class RobotSensors : public maze::Sensors {
    public:
        explicit RobotSensors(RobotContainer& container) : c_(container) {}
        bool ready() override;
        void beginScan() override;
        bool pollScan(maze::SensorReadings& out) override;
        unsigned frontMm() override;

    private:
        RobotContainer& c_;
        static constexpr int kColors = 8;
        bool active_ = false;
        unsigned long start_ = 0;
        int n_ = 0, front_ = 0, left_ = 0, right_ = 0;
        int colorVotes_[kColors] = {0, 0, 0, 0, 0, 0, 0, 0};

        void sample();
};