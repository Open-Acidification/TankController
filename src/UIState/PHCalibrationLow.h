/**
 * PHCalibrationLow.h
 *
 * pH lowpoint calibration
 */
#pragma once
#include "NumberCollectorState.h"

class PHCalibrationLow : public NumCollectorState {
public:
  PHCalibrationLow(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInCalibration() {
    return true;  // disable controls during calibration
  }
  const char* name() {
    return "PHCalibrationLow";
  }
  float getCurrentValue() {
    return 0.0;
  }
  int getCurrentValuePrecision() {
    return 3;
  }
  const char* prompt() {
    return "pH-Lowpoint     ";
  };
  void setValue(float value);
};
