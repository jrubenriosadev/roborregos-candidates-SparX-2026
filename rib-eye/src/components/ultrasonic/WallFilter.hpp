#ifndef DEF_WALLFILTER
#define DEF_WALLFILTER

#include "stdint.h"
#include "Arduino.h"

class WallFilter {
    private:
        float _h[3] = {0,0,0};
        uint8_t _i = 0, _n = 0, _miss = 0;
    public:
        static constexpr uint8_t kMax = 2; // lecturas ivalidas

        void push(float cm, bool v) {
            if(v) {
                _h[_i] = cm;
                _i = (_i+1) % 3;
                if(_n < 3) _n++;
                _miss = 0;
            } else {
                if(_miss < 255) _miss++;
                if(_miss >= kMax) { _n = 0; _i = 0; } // lo voldida
            }
        }

        bool valid() const { return _n > 0;}

        float value() const {
            if(_n == 0) return 0.0f;
            if(_n == 1) return _h[0];
            if(_n == 2) return 0.5f * (_h[0] + _h[1]);
            float a = _h[0], b = _h[1], c = _h[2];
            if((a >= b && a <= c) || (a <= b && a >= c)) return a;
            if((b >= a && b <= c) || (b <= a && b >= c)) return b;
            return c;
        }

};
#endif