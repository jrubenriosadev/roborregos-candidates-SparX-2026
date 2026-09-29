#include "RobotContainer.hpp"

RobotContainer::RobotContainer() {}

void RobotContainer::init() {
    Serial.println(" === init === ");

    if (_bno.init(Wire)) {
        Serial.println("BNO055 :)");
    } else {
        Serial.println("BNO055 :[");
    }

    _drive.init();
    Serial.println("Drive :)");
}

void RobotContainer::update() {
    _bno.update();
}

Drive& RobotContainer::getDrive() {
    return _drive;
}

Bno& RobotContainer::getBno() {
    return _bno;
}