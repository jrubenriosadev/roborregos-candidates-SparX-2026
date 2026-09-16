#ifndef DEF_MOTOR
#define DEF_MOTOR

#include <Arduino.h>

class Motor {
    public:
        Motor(uint8_t pinIN1, uint8_t pinIN2, uint8_t pinEncoderA, uint8_t pinEncoderB);

        void init();
        void forward();
        void backward();
        void stop();

        long getEncoder() const;
        void resetEncoder();
        bool isEncoderC() const;

    private:
        const uint8_t pinIN1;
        const uint8_t pinIN2;
        const uint8_t pinEncoderA;
        const uint8_t pinEncoderB;

        volatile long e_count;
        volatile bool isEncoding;

        void handleEncoder();

        static void encoderISR();
        static Motor* a_motor;
};

#endif