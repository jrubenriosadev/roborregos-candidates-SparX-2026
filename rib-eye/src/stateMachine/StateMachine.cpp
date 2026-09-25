#include "StateMachine.hpp"
#include <Arduino.h>

StateMachine::StateMachine(RobotContainer& container)
    : _container(container),
      _currentState(States::INIT),
      _pst(0) {}

void StateMachine::update() {
    static unsigned long lastPrint = 0;

    switch(_currentState) {
        case States::INIT:
            Serial.println("[STATE] INIT -> Reasegurando ganancias PID");
            _container.getDrive().setLeftForwardGains(600.0f, 0.0f, 0.0f);
            _container.getDrive().prepDistance(0.5f);
            _currentState = States::MOVE;
            break;

        case States::MOVE:
            if (millis() - lastPrint >= 150) {
                lastPrint = millis();
                Serial.print("Dist L: ");
                Serial.print(_container.getDrive().getLeftDistance(), 3);
                Serial.print(" m | Dist R: ");
                Serial.print(_container.getDrive().getRightDistance(), 3);
                Serial.print(" m");
            }

            // === PRUEBA DE DIAGNÓSTICO ===
            // Si quieres verificar si los motores se mueven físicamente SIN PID,
            // descomenta la siguiente línea y comenta la llamada a moveToDistance():
             _container.getDrive().setOpenLoop(200, 200);

            /*
            if (_container.getDrive().moveToDistance()) {
                Serial.println("\n[STATE] ¡Meta alcanzada!");
                _pst = millis();
                _currentState = States::IDLE;
            }*/
            break;

        case States::IDLE:
            if (millis() - _pst >= 1000) {
                Serial.println("\n[STATE] Reiniciando ciclo de movimiento...");
                _container.getDrive().prepDistance(0.5f);
                _currentState = States::MOVE;
            }
            break;
    }
}