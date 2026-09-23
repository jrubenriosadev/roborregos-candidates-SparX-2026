#include "PIDController.hpp"
#include <math.h>

PIDController::PIDController(float kp, float ki, float kd, float minOutput, float maxOutput)
    : _kp(kp), _ki(ki), _kd(kd), _minOuput(minOutput), _maxOutput(maxOutput),
      integral_pp(0.0f), lastMeasurement(0.0f), lastErr(0.0f), lastOutput(0.0f),
      lastMicros(0), _isEnabled(true), _angleWrapping(false) {}
  
void PIDController::reset() {
  integral_pp = 0.0f;
  lastMeasurement = 0.0f;
  lastErr = 0.0f;
  lastOutput = 0.0f;
  lastMicros = 0;
}

float PIDController::CWErr(float setpoint, float measurement) const {
  float err = setpoint - measurement;
  
  while (err > M_PI) { err -= 2.0f * M_PI; }
  while (err < -M_PI) { err += 2.0f * M_PI; }

  return err;
}

float PIDController::update(float measurement, float setpoint) {
  unsigned long m = micros();

  if(lastMicros == 0) {
    lastMicros = m;
    lastMeasurement = measurement;
    return 0.0f;
  }

  float dt = (float)(m - lastMicros) / 1000000.0f;
  lastMicros = m;

  return update(measurement, setpoint, dt);
}

float PIDController::update(float measurement, float setpoint, float dt) {
  if (!_isEnabled || dt <= 0.0f) {
    return 0.0f;
  }

  float err = _angleWrapping ? CWErr(setpoint, measurement) : (setpoint - measurement);
  lastErr = err;

  float pT = _kp * err;
  integral_pp += err * dt;

  float maxI = (_ki > 0.0f) ? (_maxOutput / _ki) : 0.0f;
  integral_pp = constrain(integral_pp, -maxI, maxI);
  float iT = _ki * integral_pp;

  float d = (measurement - lastMeasurement) / dt;
  float dT = -_kd * d;
  lastMeasurement = measurement;

  lastOutput = constrain(pT + iT + dT, _minOuput, _maxOutput);
  return lastOutput;
}

void PIDController::setGains(float kp, float ki, float kd) {
  _kp = kp;
  _ki = ki;
  _kd = kd;
}

void PIDController::setOutputLimits(float minOutput, float maxOutput) {
  _minOuput = minOutput;
  _maxOutput = maxOutput;
}

void PIDController::setEnabled(bool e) {
  _isEnabled = e;
  if(!_isEnabled) reset();
}

void PIDController::setAngleWrapping(bool e) { _angleWrapping = e; }
bool PIDController::isEnabled() const { return _isEnabled; }
bool PIDController::isAnlgeWrapping() const { return _angleWrapping; }
float PIDController::getErr() const { return lastErr; }
float PIDController::getOutput() const { return lastOutput; }
float PIDController::getKp() const { return _kp; }
float PIDController::getKi() const { return _ki; }
float PIDController::getKd() const { return _kd; }