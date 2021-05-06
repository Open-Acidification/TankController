/**
 * PHCalibrationHigh.h
 *
 * pH highpoint calibration
 */
#pragma once
#include "NumberCollectorState.h"

class PHCalibrationHigh : public NumCollectorState {
public:
  PHCalibrationHigh(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInCalibration() {
    return true;  // disable controls during calibration
  }
  const char* name() {
    return "PHCalibrationHigh";
  }
  float getCurrentValue() {
    return 0.0;
  }
  int getCurrentValuePrecision() {
    return 3;
  }
  const char* prompt() {
    return "pH-Highpoint    ";
  };
  void setValue(double value);
};
