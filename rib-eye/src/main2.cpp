#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "BluetoothSerial.h"

#define MAZE_CHALLENGE 1

#if MAZE_CHALLENGE
#include "maze/hardware/MazeApp.hpp"
#else
#include "stateMachine/StateMachine.hpp"
#endif

BluetoothSerial bluetooth;
RobotContainer container;
#if MAZE_CHALLENGE
MazeApp mazeApp(container);
#else
StateMachine stateMachine(container);
#endif

constexpr unsigned long kControlPeriod = 20000UL;
unsigned long nextTick = 0;
unsigned long lastPrint = 0;

void recoverI2C(uint8_t sdaPin, uint8_t sclPin) {
    pinMode(sdaPin, INPUT_PULLUP);
    pinMode(sclPin, OUTPUT);
    for (int i = 0; i < 10; i++) {
        digitalWrite(sclPin, LOW);
        delayMicroseconds(5);
        digitalWrite(sclPin, HIGH);
        delayMicroseconds(5);
    }
    pinMode(sdaPin, INPUT);
    pinMode(sclPin, INPUT);
}

void setup() {
    Serial.begin(115200);
    bluetooth.begin("RIBEYE");
    while (!Serial && millis() < 3000);

    recoverI2C(21, 22);
    recoverI2C(32, 23);

    Wire.begin(21, 22);
    Wire.setClock(100000);
    Wire.setTimeOut(50);

    Wire1.begin(32, 23);
    Wire1.setClock(100000);
    Wire1.setTimeOut(50);

    container.init();
    container.getDrive().stop();
    nextTick = micros();
}

void loop() {
    const unsigned long now = micros();
    if ((long)(now - nextTick) < 0) return;
    nextTick += kControlPeriod;
    if ((long)(now - nextTick) > (long)kControlPeriod) nextTick = now + kControlPeriod;

#if MAZE_CHALLENGE
    mazeApp.update();
#else
    container.update();
    stateMachine.update();
#endif

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        char buffer[200];
#if MAZE_CHALLENGE
        const maze::MazeStateMachine& m = mazeApp.machine();
        const maze::GridPose& p = m.pose();
        snprintf(buffer, sizeof(buffer),
                 "%s/%s pos(%d,%d) hd:%s trj:%u vis:%d fin:%s err:%s",
                 maze::stateName(m.state()), m.phaseName(),
                 p.position.x, p.position.y, maze::directionName(p.heading),
                 (unsigned)m.trajectory().size(), m.map().visitedCount(),
                 m.finishFound() ? "SI" : "no", maze::errorName(m.error()));
#else
        snprintf(buffer, sizeof(buffer), "Yaw:%7.2f | ToF L:%4u R:%4u mm | US L:%5.1f R:%5.1f cm",
                 container.getBno().getRelativeYaw(),
                 container.getTofLeftMm(), container.getTofRightMm(),
                 container.getLeftDistance(), container.getRightDistance());
#endif
        Serial.println(buffer);
        bluetooth.println(buffer);
    }
}