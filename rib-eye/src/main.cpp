#define TEST_MODE 4  // 1=color  2=openloop  3=tile PID  4=statemachine

#include <Arduino.h>
#include "container/RobotContainer.hpp"
#include "BluetoothSerial.h"

#if TEST_MODE == 4
#include "stateMachine/StateMachine.hpp"
#endif

BluetoothSerial bluetooth;
RobotContainer container;
#if TEST_MODE == 4
StateMachine stateMachine(container);
#endif

constexpr unsigned long kControlPeriod = 20000UL;
unsigned long nextTick = 0;
unsigned long lastPrint = 0;

void say(const char* msg) {
    Serial.println(msg);
    bluetooth.println(msg);
}

char readCommand() {
    if (Serial.available()) return (char)Serial.read();
    if (bluetooth.available()) return (char)bluetooth.read();
    return 0;
}

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

const char* colorName(ColorDetected c) {
    switch (c) {
        case ColorDetected::RED:     return "ROJO";
        case ColorDetected::GREEN:   return "VERDE";
        case ColorDetected::CYAN:    return "CYAN";
        case ColorDetected::YELLOW:  return "AMARILLO";
        case ColorDetected::ORANGE:  return "NARANJA";
        case ColorDetected::MAGENTA: return "MAGENTA";
        default:                     return "NINGUNO";
    }
}

// ---------------------------------------------------------------- 1: COLOR
#if TEST_MODE == 1
void testSetup() {
    say("--- TEST COLOR (motores apagados) ---");
}

void testLoop() {
    container.getDrive().stop();
    const ColorDetected c = container.getColorDetected();

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        const RGB rgb = container.getLastRGB();
        char b[100];
        snprintf(b, sizeof(b), "R:%6.1f G:%6.1f B:%6.1f => %s", rgb.r, rgb.g, rgb.b, colorName(c));
        say(b);
    }
}

// ------------------------------------------------------------- 2: OPENLOOP
#elif TEST_MODE == 2
int pwm = 100;
int cmdL = 0, cmdR = 0;
unsigned long cmdStart = 0;
constexpr unsigned long kAutoStopMs = 3000;

void apply(int l, int r) {
    cmdL = l; cmdR = r;
    cmdStart = millis();
    container.getDrive().setOpenLoop(l, r);
}

void testSetup() {
    say("--- TEST OPENLOOP ---");
    say("m adelante | b atras | l giro izq | r giro der | s stop | + / - cambia PWM (+-10)");
    say("Se detiene solo a los 3 s");
}

void testLoop() {
    const char c = readCommand();
    switch (c) {
        case 'm': apply(pwm, pwm); break;
        case 'b': apply(-pwm, -pwm); break;
        case 'l': apply(-pwm, pwm); break;
        case 'r': apply(pwm, -pwm); break;
        case 's': apply(0, 0); container.getDrive().stop(); break;
        case '+': pwm = constrain(pwm + 10, 0, 255); { char b[30]; snprintf(b, sizeof(b), "PWM=%d", pwm); say(b); } break;
        case '-': pwm = constrain(pwm - 10, 0, 255); { char b[30]; snprintf(b, sizeof(b), "PWM=%d", pwm); say(b); } break;
        default: break;
    }

    if ((cmdL != 0 || cmdR != 0) && millis() - cmdStart > kAutoStopMs) {
        cmdL = cmdR = 0;
        container.getDrive().stop();
        say(">> auto-stop");
    }

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        Drive& d = container.getDrive();
        char b[140];
        snprintf(b, sizeof(b), "cmd L:%4d R:%4d | encL:%6ld encR:%6ld | distL:%6.3f distR:%6.3f m | yaw:%7.2f",
                 cmdL, cmdR, d.getLeftEncoder(), d.getRightEncoder(),
                 d.getLeftDistance(), d.getRightDistance(),
                 container.getBno().getRelativeYaw());
        say(b);
    }
}

// ------------------------------------------------------------ 3: TILE PID
#elif TEST_MODE == 3
enum class TS { IDLE, MOVING, PAUSE };
TS ts = TS::IDLE;
float lockedYaw = 0.0f;
bool continuous = false;
unsigned long moveStart = 0, pauseStart = 0;
int tilesDone = 0;
constexpr unsigned long kTileTimeoutMs = 5000;
constexpr unsigned long kPauseMs = 1000;

float yawErr(float a, float b) {
    float e = a - b;
    while (e > 180.0f) e -= 360.0f;
    while (e <= -180.0f) e += 360.0f;
    return e;
}

void startTile() {
    container.getDrive().prepMoveTile(DriveConfig::kTSize, lockedYaw);
    moveStart = millis();
    ts = TS::MOVING;
}

void testSetup() {
    delay(500);
    container.update();
    const float y = container.getBno().getRelativeYaw();
    lockedYaw = isfinite(y) ? y : 0.0f;
    say("--- TEST TILE 0.3 m (PID) ---");
    say("g un tile | c continuo on/off | z fijar rumbo actual | s stop");
    char b[60];
    snprintf(b, sizeof(b), "rumbo fijado: %.2f", lockedYaw);
    say(b);
}

void testLoop() {
    const char c = readCommand();
    if (c == 'g' && ts == TS::IDLE) { tilesDone = 0; startTile(); }
    else if (c == 'c') {
        continuous = !continuous;
        say(continuous ? ">> continuo ON (manda g para empezar)" : ">> continuo OFF");
    }
    else if (c == 'z') {
        const float y = container.getBno().getRelativeYaw();
        if (isfinite(y)) { lockedYaw = y; char b[50]; snprintf(b, sizeof(b), "rumbo fijado: %.2f", y); say(b); }
    }
    else if (c == 's') {
        container.getDrive().stop();
        ts = TS::IDLE;
        continuous = false;
        say(">> STOP");
    }

    Drive& d = container.getDrive();
    const float yaw = container.getBno().getRelativeYaw();

    switch (ts) {
        case TS::IDLE:
            d.stop();
            break;

        case TS::MOVING: {
            const bool done = d.moveTile(yaw, container.getLeftDistance(), container.getRightDistance(), DriveConfig::kBSpeed);
            const bool timeout = millis() - moveStart > kTileTimeoutMs;
            if (done || timeout) {
                d.stop();
                ++tilesDone;
                char b[160];
                snprintf(b, sizeof(b), ">> %s tile %d | %lu ms | L:%.3f R:%.3f m | yaw:%.2f | err rumbo:%+.2f deg",
                         done ? "LISTO" : "TIMEOUT", tilesDone, millis() - moveStart,
                         d.getLeftDistance(), d.getRightDistance(), yaw, yawErr(lockedYaw, yaw));
                say(b);
                pauseStart = millis();
                ts = (done && continuous) ? TS::PAUSE : TS::IDLE;
            }
            break;
        }

        case TS::PAUSE:
            d.stop();
            if (millis() - pauseStart >= kPauseMs) startTile();
            break;
    }

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        char b[140];
        snprintf(b, sizeof(b), "[%s] yaw:%7.2f obj:%7.2f | L:%.3f R:%.3f m | US L:%5.1f R:%5.1f cm",
                 ts == TS::IDLE ? "IDLE" : ts == TS::MOVING ? "MOVE" : "WAIT",
                 yaw, lockedYaw, d.getLeftDistance(), d.getRightDistance(),
                 container.getLeftDistance(), container.getRightDistance());
        say(b);
    }
}

// ----------------------------------------------------------- 4: STATEMACHINE
#elif TEST_MODE == 4
void testSetup() {
    say("--- STATEMACHINE NORMAL ---");
}

void testLoop() {
    stateMachine.update();

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        char b[160];
        snprintf(b, sizeof(b), "Yaw:%7.2f | ToF L:%4u R:%4u mm | US L:%5.1f R:%5.1f cm | %s",
                 container.getBno().getRelativeYaw(),
                 container.getTofLeftMm(), container.getTofRightMm(),
                 container.getLeftDistance(), container.getRightDistance(),
                 colorName(container.getColorDetected()));
        say(b);
    }
}
#else
#error "TEST_MODE debe ser 1, 2, 3 o 4"
#endif

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

    testSetup();
    nextTick = micros();
}

void loop() {
    const unsigned long now = micros();
    if ((long)(now - nextTick) < 0) return;
    nextTick += kControlPeriod;
    if ((long)(now - nextTick) > (long)kControlPeriod) nextTick = now + kControlPeriod;

    container.update();
    testLoop();
}