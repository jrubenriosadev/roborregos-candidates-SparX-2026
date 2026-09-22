#ifndef DEF_PID_CONTROLLER
#define DEF_PID_CONTROLLER

#include <Arduino.h>

//TODO: Test
class PIDController {
    public:
        PIDController(
            float kp = 0.0f, 
            float ki = 0.0f, 
            float kd = 0.0f,
            float minOutput = -255.0f, 
            float maxOutput = 255.0f);

        ~PIDController() = default;
        
        float update(float measurement, float setpoint);
        float update(float measurement, float setpoint, float dt); // explicit

        void reset();

        // S
        void setGains(float kp, float ki, float kd);
        void setOutputLimits(float minOutput, float maxOutput);
        void setEnabled(bool e);
        void setAngleWrapping(bool e);

        // G
        bool isEnabled() const;
        bool isAnlgeWrapping() const;
        float getErr() const;
        float getOutput() const;
        float getKp() const;
        float getKi() const;
        float getKd() const;

        float CWErr(float setpoint, float measurement) const; // compute

    private:
        float _kp;
        float _ki;
        float _kd;
        float _minOuput;
        float _maxOutput;

        float integral_pp;
        float lastMeasurement;
        float lastErr;
        float lastOutput;

        unsigned long lastMicros;
        bool _isEnabled;
        bool _angleWrapping;

};

#endif