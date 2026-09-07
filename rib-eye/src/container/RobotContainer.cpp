#include "RobotContainer.h"

RobotContainer::RobotContainer() {}

void RobotContainer::init() {
    drive.init();
}

void RobotContainer::update() {
    drive.forward(1000);
    drive.stop(100);
    drive.turnRight();
    drive.stop(100);
}