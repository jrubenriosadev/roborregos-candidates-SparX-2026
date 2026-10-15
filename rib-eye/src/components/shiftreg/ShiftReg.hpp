#ifndef DEF_SHIFTREG
#define DEF_SHIFTREG

#include "Arduino.h"

class ShiftReg {
    public:
        static void init(uint8_t data, uint8_t clock, uint8_t latch, uint8_t oe);
        static void write(uint8_t bit, bool level);
        static void flush(bool force = false);
};

#endif