#include "RobotContainer.hpp"

RobotContainer::RobotContainer() {}

void RobotContainer::init() {
    Serial.println("  [CONTAINER] 2.1 Configurando bus I2C (Wire)...");
    Wire.begin(21, 22);
    Wire.setClock(100000);

    Serial.println("  [CONTAINER] 2.2 Inicializando BNO055...");
    //_bno.init(Wire); 

    Serial.println("  [CONTAINER] 2.3 Inicializando Drive y Motores...");
    _drive.init();

    Serial.println("  [CONTAINER] 2.4 Container init completado con éxito.");
}

void RobotContainer::update() {
    //_bno.update();
}

Drive& RobotContainer::getDrive() { return _drive; }
Bno& RobotContainer::getBno() { return _bno; }