#ifndef DEF_MOTOR
#define DEF_MOTOR

#include <Arduino.h>

class Motor {
    public:
        Motor(
            uint8_t pinIN1, 
            uint8_t pinIN2, 
            uint8_t pinPWM, 
            uint8_t pinEncoderA, 
            uint8_t pinEncoderB, 
            float ppr = 400.0f, 
            float wheelDiameterM = 0.0f, 
            bool inverted = false);

        ~Motor();

        Motor(const Motor&) = delete;
        Motor& operator = (const Motor&) = delete;

        void init();

        void move(int speed);
        void forward(uint8_t speed = 255);
        void backward(uint8_t speed = 255);
        void stop();
        void brake();

        void setPPR(float ppr);
        void setWheelDiameter(float diameterM);
        void setInverted(bool i);

        float getPPR() const;
        float getWheelDiameter() const;
        bool isInverted() const;

        long getEncoder() const;
        float getRevolutions() const;
        float getRadians() const;
        float getDistanceMeters() const;
        void resetEncoder();

        float updateVelocity();
        float getRPM() const;
        float getRadSec() const;
        float getSpeedMPS() const;

        template<uint8_t I>
        static void isrWrapper();

    private:
        const uint8_t _pinIN1;
        const uint8_t _pinIN2;
        const uint8_t _pinPWM;
        const uint8_t _pinEncoderA;
        const uint8_t _pinEncoderB;

        bool _inverted;
        float _ppr;
        float _wheelDiameter;

        volatile long e_count;

        long last_count;
        unsigned long last_vel_time;
        float current_rpm;
        float current_radps;

        uint8_t _slotIndex;

        void handleEncoder();

        static constexpr uint8_t C = 8;
        static Motor* instances[C];
};

#endif