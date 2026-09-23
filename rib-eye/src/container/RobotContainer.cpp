#include "RobotContainer.hpp"

RobotContainer::RobotContainer() {}

void RobotContainer::init() {
    _mpu.init();
    _drive.init();
}

Drive& RobotContainer::getDrive() { return _drive; }
Mpu& RobotContainer::getMpu() { return _mpu; }