#include "TuningMode.hpp"
#include "Arduino.h"

TuningMode::TuningMode(RobotContainer& container)
    : container(container), kp(0.0f), ki(0.0f), kd(0.0f), setpoint(0.0f), tuningFM(true) {
        container.getDrive().prepDistance(0.0f);
    }

void TuningMode::run() {
    processSerialCmd();

    if(!tuningFM) {
        MpuData imu;
        container.getMpu().readAsync(imu);
        container.getDrive().turnToAngle(imu.yaw);
    } else {
        container.getDrive().moveToDistance();
    }
    printTelemetry();
}

void TuningMode::processSerialCmd() {
    if(Serial.available() > 0) {
        char cmd = Serial.read();
        float val = Serial.parseFloat();

        switch(cmd) {
            case 'P': kp = val; break;
            case 'I': ki = val; break;
            case 'D': kd = val; break;
            case 'S': 
                setpoint = val;
                if(tuningFM) container.getDrive().prepDistance(setpoint);
                else container.getDrive().prepAngle(setpoint); 
                break;
            case 'M': 
                tuningFM = (val == 0.0f);
                setpoint = (0.0f);
                Serial.println(tuningFM ? "FORWARD MODE" : "STEER MODE");
                break;
        }
        if(tuningFM) container.getDrive().setLeftForwardGains(kp, ki, kd);
        else container.getDrive().setSteerGains(kp, ki, kd);
        while(Serial.available()) Serial.read();
    }
}

void TuningMode::printTelemetry() {
    Serial.print(setpoint);
    if(tuningFM) {
        Serial.print(container.getDrive().getLeftDistance());
        Serial.print(" | ");
        Serial.println(container.getDrive().getRightDistance());
    } else {
        MpuData imu;
        container.getMpu().readAsync(imu);
        Serial.println(imu.yaw);
    }
}