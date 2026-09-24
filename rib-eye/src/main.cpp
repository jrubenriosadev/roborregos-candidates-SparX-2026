#include "container/RobotContainer.hpp"
#include "stateMachine/StateMachine.hpp"
#include "tuning/TuningMode.hpp"
#include "tuning/Encoders/EncoderTestMode.hpp"
#include "tuning/Mpu/MpuTestMode.hpp"

constexpr bool isTuningMode = false;
constexpr bool isEncodersTest = false;
constexpr bool isMpuTest = true;

RobotContainer container;
StateMachine stateMachine(container);
TuningMode tun(container);
EncoderTestMode enc(container);
MpuTestMode mpuTest(container);

void setup() {
    Serial.begin(115200);
    container.init();
}

void loop() {
    if(isTuningMode) {
        tun.run();
    } else if(isEncodersTest) {
        enc.run();
    } else if(isMpuTest) {
        mpuTest.run();
    } else {
        stateMachine.update();
    }
}