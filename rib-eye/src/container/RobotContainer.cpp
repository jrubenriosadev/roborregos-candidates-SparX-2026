#include "RobotContainer.hpp"

RobotContainer::RobotContainer() {}

void RobotContainer::init() {

    Serial.println(" === init === ");

    Wire.begin(21, 22);
    Wire.setClock(100000);

    Serial.println("test1");

    /*if (_bno.init(Wire)) {
        Serial.println("BNO055 :)");
    } else {
        Serial.println("BNO055 :[");
    }*/

    Serial.println("test2");

    _drive.init();

    Serial.println("test 3: drive :)");
}

void RobotContainer::update() {
    //_bno.update();
}

Drive& RobotContainer::getDrive() {
    return _drive;
}

Bno& RobotContainer::getBno() {
    return _bno;
}