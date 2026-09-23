#include "TuningMode.hpp"
#include "Arduino.h"

TuningMode::TuningMode(RobotContainer& container)
    : container(container), kp(0.0f), ki(0.0f), kd(0.0f), setpoint(0.0f), tuningFM(true) {
        container.getDrive().prepDistance(0.0f);
    }

void TuningMode::run() {
    
}