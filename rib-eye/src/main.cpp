// 1 color, 2 open, 3 tile, 4 prestatemachine, 5 maze, 6 sensores, 7 fase 1, 8 fase 2, 9 fase 3, 10 levels
#define TEST_MODE 10

#include <Arduino.h>
#include "container/RobotContainer.hpp"

#if TEST_MODE == 4
#include "stateMachine/StateMachine.hpp"
#elif TEST_MODE == 5
#include "challenges/maze/hardware/MazeMain.hpp"
#elif TEST_MODE >= 7 && TEST_MODE <= 10
#include "challenges/levels/Levels.hpp"
#endif

RobotContainer container;
#if TEST_MODE == 4
StateMachine stateMachine(container);
#elif TEST_MODE == 5
MazeMain mazeMain(container);
#elif TEST_MODE >= 7 && TEST_MODE <= 10
Levels levelsRun(container);
#endif

constexpr unsigned long kControlPeriod = 20000UL;
unsigned long nextTick = 0;
unsigned long lastPrint = 0;

void say(const char* msg) {
    Serial.println(msg);
}

char readCommand() {
    if (Serial.available()) return (char)Serial.read();
    return 0;
}

void i2cScan(TwoWire& bus, const char* name) {
    Serial.printf("Scan %s:", name);
    int n = 0;
    for (uint8_t a = 1; a < 127; a++) {
        bus.beginTransmission(a);
        if (bus.endTransmission() == 0) { Serial.printf(" 0x%02X", a); n++; }
    }
    Serial.println(n ? "" : " (nada)");
}

void recoverI2C(uint8_t sdaPin, uint8_t sclPin) {
    pinMode(sdaPin, INPUT_PULLUP);
    pinMode(sclPin, OUTPUT);
    
    for (int i = 0; i < 16; i++) {
        digitalWrite(sclPin, LOW);
        delayMicroseconds(10);
        digitalWrite(sclPin, HIGH);
        delayMicroseconds(10);
        if (digitalRead(sdaPin) == HIGH) break; // SDA liberado
    }

    pinMode(sdaPin, OUTPUT);
    digitalWrite(sdaPin, LOW);
    delayMicroseconds(10);
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sdaPin, HIGH);
    delayMicroseconds(10);

    pinMode(sdaPin, INPUT);
    pinMode(sclPin, INPUT);
}

const char* colorName(ColorDetected c) {
    switch (c) {
        case ColorDetected::RED:return "ROJO";
        case ColorDetected::GREEN:return "VERDE";
        case ColorDetected::CYAN:return "CYAN";
        case ColorDetected::YELLOW:return "AMARILLO";
        case ColorDetected::ORANGE:return "NARANJA";
        case ColorDetected::MAGENTA:return "MAGENTA";
        case ColorDetected::LIME:return "LIME";
        default:return "NINGUNO";
    }
}

#if TEST_MODE == 1
void testSetup() {
    say("COLOR");
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
    say("OPEN LOOP");
}

void testLoop() {
    apply(100,100);

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        Drive& d = container.getDrive();
        char b[140];
        snprintf(b, sizeof(b), "cmd L:%4d R:%4d | encL:%6ld encR:%6ld | distL:%6.3f distR:%6.3f m | yaw:%7.2f",  cmdL, cmdR, d.getLeftEncoder(), d.getRightEncoder(), d.getLeftDistance(), d.getRightDistance(), container.getBno().getRelativeYaw());
        say(b);
    }
}

#elif TEST_MODE == 3

void testSetup() {
}

void testLoop() {
    
}

#elif TEST_MODE == 4
void testSetup() {
    say("NORMAL");
}

void testLoop() {
    stateMachine.update();

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        char b[160];
        snprintf(b, sizeof(b), "Yaw:%7.2f | ToF L:%4u| US L:%5.1f R:%5.1f cm | %s",
                 container.getBno().getRelativeYaw(),
                 container.getTofLeftMm(),
                 container.getLeftDistance(), container.getRightDistance(),
                 colorName(container.getColorDetected()));
        say(b);
    }
}

#elif TEST_MODE == 5
void testSetup() {
    say("--- MAZE ---");
}

void testLoop() {
    mazeMain.update();

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        const maze::MazeStateMachine& m = mazeMain.machine();
        const maze::GridPose& g = m.pose();
        char b[160];
        snprintf(b, sizeof(b), "%s/%s pos(%d,%d) hd:%s trj:%u vis:%d fin:%s err:%s", maze::stateName(m.state()), m.phaseName(),g.p.x, g.p.y, maze::d_name(g.h), (unsigned)m.trajectory().size(), m.map().visitedCount(), m.finishFound() ? "SI" : "no", maze::errorName(m.error()));
        say(b);
    }
}

#elif TEST_MODE == 6
void testSetup() {
    say("SENSORES");
}

void testLoop() {
    container.getDrive().stop();

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        char b[280];
        snprintf(b, sizeof(b), "Yaw:%7.2f | ToF L:%4u mm rdy:%d err:%lu| US L:%5.1f R:%5.1f cm | TCRT L:%4u%s R:%4u%s | %s", container.getBno().getRelativeYaw(), container.getTofLeftMm(), (int)container.tofReady(), (unsigned long)container.getTofL().getErrorCount(), container.getLeftDistance(), container.getRightDistance(), container.lineLeftRaw(), container.lineLeftWhite() ? "(B)" : "(v)", container.lineRightRaw(), container.lineRightWhite() ? "(B)" : "(v)", colorName(container.getColorDetected()));
        say(b);
    }
}

#elif TEST_MODE >= 7 && TEST_MODE <= 10
void testSetup() {
#if TEST_MODE == 7
    say("FASE 1");
    levelsRun.arm(levels::LevelsPhase::SECTION1, ColorDetected::LIME, false);
#elif TEST_MODE == 8
    say("FASE 2");
    levelsRun.arm(levels::LevelsPhase::SECTION2, ColorDetected::NONE, false);
#elif TEST_MODE == 9
    say("FASE 3");
    levelsRun.arm(levels::LevelsPhase::SECTION3, ColorDetected::RED, false);
#else
    say("LEVELS");
    levelsRun.arm(levels::LevelsPhase::SECTION1, ColorDetected::LIME, true);
#endif
    say("'g' inicia | 's' paro | 'z' fija el rumbo actual como 0 global (robot de frente a la pista)");
}

void testLoop() {
    const char c = readCommand();
    if (c == 'g') levelsRun.go();
    else if (c == 's') levelsRun.abort();
    else if (c == 'z') {
        container.getBno().resetHeading();
        say("rumbo global fijado en 0");
    }

    levelsRun.update();

    if (millis() - lastPrint >= 250) {
        lastPrint = millis();
        char b[200];
        const float yaw = container.getBno().getRelativeYaw();
        switch (levelsRun.state()) {
            case levels::LevelsState::SECTION1: {
                const levels::Section1& s = levelsRun.section1();
                snprintf(b, sizeof(b), "S1/%s pos(%d,%d) hd:%s pelota:%s ToF:%4u | yaw:%7.2f | err:%s",
                         levels::stateName(s.state()), s.pose().p.x, s.pose().p.y, maze::d_name(s.pose().h),
                         s.hasBall() ? "SI" : "no", container.getTofLeftMm(), yaw, levels::errorName(s.error()));
                break;
            }
            case levels::LevelsState::SECTION2: {
                const levels::Section2& s = levelsRun.section2();
                snprintf(b, sizeof(b), "S2/%s carril:%+d avance:%.2f m | TCRT L:%4u%s R:%4u%s | yaw:%7.2f | err:%s",
                         levels::stateName(s.state()), s.lane(), s.progress(),
                         container.lineLeftRaw(), container.lineLeftWhite() ? "(B)" : "(v)",
                         container.lineRightRaw(), container.lineRightWhite() ? "(B)" : "(v)",
                         yaw, levels::errorName(s.error()));
                break;
            }
            case levels::LevelsState::SECTION3: {
                const levels::Section3& s = levelsRun.section3();
                snprintf(b, sizeof(b), "S3/%s casillas:%d obj:%7.2f yaw:%7.2f | %s | err:%s",
                         levels::stateName(s.state()), s.tiles(), s.targetYaw(), yaw,
                         colorName(container.getColorDetected()), levels::errorName(s.error()));
                break;
            }
            default:
                snprintf(b, sizeof(b), "%s | yaw:%7.2f | %s", levels::stateName(levelsRun.state()), yaw,
                         colorName(container.getColorDetected()));
                break;
        }
        say(b);
    }
}
#else
#error "TEST_MODE: 1 a 10"
#endif

void setup() {
    pinMode(Pins::ToF::kLeftXShut, OUTPUT);
    digitalWrite(Pins::ToF::kLeftXShut, LOW);

    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    recoverI2C(21, 22);
    recoverI2C(23, 18);

    Wire.begin(21, 22);
    Wire.setClock(100000);
    Wire.setTimeOut(10);

    Wire1.begin(23, 18);
    Wire1.setClock(100000);
    Wire1.setTimeOut(10);

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