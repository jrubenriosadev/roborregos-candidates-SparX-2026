#include "RobotContainer.hpp"

RobotContainer::RobotContainer() {}

void RobotContainer::init() {
    Serial.println(" === init === ");
    Wire.begin(21, 22);
    Wire.setClock(100000);

    Serial.println("test1");
    //_bno.init(Wire); 

    Serial.println("test2");
    _drive.init();

    Serial.println("test 3");
}

void RobotContainer::update() {
    //_bno.update();
}

Drive& RobotContainer::getDrive() { return _drive; }
Bno& RobotContainer::getBno() { return _bno; }