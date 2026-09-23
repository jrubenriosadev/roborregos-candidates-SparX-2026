#include "EncoderTestMode.hpp"
#include "Arduino.h"

EncoderTestMode::EncoderTestMode(RobotContainer& container)
    : _container(container), _lpt(0) {}

void EncoderTestMode::run() {
    _container.getDrive().stop();

    if(millis() - _lpt >= 100) {
        _lpt = millis();

        long lt = _container.getDrive().getLeftTicks();
        long rt = _container.getDrive().getRightTicks();

        float ld = _container.getDrive().getLeftDistance();
        float rd = _container.getDrive().getRightDistance();

        Serial.print("Ticks L: ");
        Serial.print(lt);
        Serial.print(" | Ticks R: ");
        Serial.print(rt);
        Serial.print(" | Dist L: ");
        Serial.print(ld, 3);
        Serial.print(" | Dist R: ");
        Serial.println(rd, 3);
    }
}