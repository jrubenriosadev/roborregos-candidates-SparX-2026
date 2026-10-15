#include "ShiftReg.hpp"

namespace {
    uint8_t sData = 0, sClock = 0, sLatch = 0, sOE = 0;
    uint8_t sState = 0, sSent = 0;
}

void ShiftReg::init(uint8_t data, uint8_t clock, uint8_t latch, uint8_t oe) {
    sData = data;
    sClock = clock;
    sLatch = latch;
    sOE = oe;
    pinMode(sOE, OUTPUT);
    digitalWrite(sOE, HIGH);
    pinMode(sData, OUTPUT);
    pinMode(sClock, OUTPUT);
    pinMode(sLatch, OUTPUT);
    sState = 0;
    flush(true);
    digitalWrite(sOE, LOW);
}

void ShiftReg::write(uint8_t bit, bool level) {
    if(bit > 7) return;
    if(level) sState |= (uint8_t)(1u << bit);
    else sState &= (uint8_t)~(1u<<bit);
}

void ShiftReg::flush(bool force) {
    if(!force && sState == sSent) return;
    digitalWrite(sLatch, LOW);
    shiftOut(sData, sClock, MSBFIRST, sState);
    digitalWrite(sLatch, HIGH);
    sSent = sState;
}