#include "TuningMode.hpp"
#include <Arduino.h>

TuningMode::TuningMode(RobotContainer& container)
    : container(container), kp(600.0f), ki(0.0f), kd(0.0f), 
      stepDistance(0.5f), tuningFM(true), autoStepMode(false), 
      isMoving(false), pauseStartTime(0) {}

void TuningMode::init() {
    container.getDrive().setLeftForwardGains(kp, ki, kd);
    printMenu();
}

void TuningMode::printMenu() {
    Serial.println("\n==========================================");
    Serial.println("      MODO TUNING PASO A PASO READY       ");
    Serial.println("==========================================");
    Serial.println("Comandos en Terminal:");
    Serial.println("  N       -> Dar 1 paso manual (0.5m)");
    Serial.println("  A       -> Activar / Pausar modo auto");
    Serial.println("  P600    -> Cambiar Kp a 600");
    Serial.println("  I20     -> Cambiar Ki a 20");
    Serial.println("  D0.5    -> Cambiar Kd a 0.5");
    Serial.println("  S0.8    -> Cambiar paso a 0.8m");
    Serial.println("  X       -> PARO DE EMERGENCIA");
    Serial.println("==========================================\n");
}

void TuningMode::triggerNextStep() {
    isMoving = true;
    if (tuningFM) {
        // Llama a prepDistance del subsistema Drive
        container.getDrive().prepDistance(stepDistance);
        Serial.print("\n[PASO] Avanzando ");
        Serial.print(stepDistance);
        Serial.println(" m...");
    } else {
        container.getDrive().prepAngle(stepDistance);
        Serial.print("\n[PASO] Girando ");
        Serial.print(stepDistance);
        Serial.println(" deg...");
    }
}

void TuningMode::run() {
    processSerialCmd();

    if (tuningFM) {
        if (isMoving) {
            // Llama a moveToDistance del subsistema Drive
            bool reached = container.getDrive().moveToDistance();
            if (reached) {
                isMoving = false;
                pauseStartTime = millis();
                Serial.println("[PASO] ¡Posición alcanzada!");
            }
        } else if (autoStepMode) {
            if (millis() - pauseStartTime >= 1000) {
                triggerNextStep();
            }
        }
    } else {
        float currentYaw = container.getBno().getEuler().x();
        container.getDrive().turnToAngle(currentYaw);
    }

    printTelemetry();
}

void TuningMode::processSerialCmd() {
    while (Serial.available() > 0) {
        char cmd = Serial.read();

        if (cmd == '\n' || cmd == '\r' || cmd == ' ') continue;

        cmd = toupper(cmd);

        if (cmd == 'P' || cmd == 'I' || cmd == 'D' || cmd == 'S') {
            float val = Serial.parseFloat();
            if (cmd == 'P') { kp = val; Serial.print("[PID] Kp = "); Serial.println(kp); }
            else if (cmd == 'I') { ki = val; Serial.print("[PID] Ki = "); Serial.println(ki); }
            else if (cmd == 'D') { kd = val; Serial.print("[PID] Kd = "); Serial.println(kd); }
            else if (cmd == 'S') { stepDistance = val; Serial.print("[CONFIG] Paso = "); Serial.println(stepDistance); }

            if (tuningFM) container.getDrive().setLeftForwardGains(kp, ki, kd);
            else container.getDrive().setSteerGains(kp, ki, kd);
        } 
        else if (cmd == 'N') {
            autoStepMode = false;
            triggerNextStep();
        } 
        else if (cmd == 'A') {
            autoStepMode = !autoStepMode;
            Serial.print("[AUTO STEP] ");
            Serial.println(autoStepMode ? "ACTIVADO" : "PAUSADO");
            if (autoStepMode && !isMoving) triggerNextStep();
        } 
        else if (cmd == 'X') {
            autoStepMode = false;
            isMoving = false;
            container.getDrive().stop();
            Serial.println("[STOP] Robot detenido");
        }
    }
}

void TuningMode::printTelemetry() {
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 150) {
        lastPrint = millis();
        if (isMoving) {
            Serial.print("Target: "); Serial.print(stepDistance);
            Serial.print(" m | L: "); Serial.print(container.getDrive().getLeftDistance(), 3);
            Serial.print(" m | R: "); Serial.print(container.getDrive().getRightDistance(), 3);
            Serial.println(" m");
        }
    }
}