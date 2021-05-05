/**
 * PHCalibrationMid.h
 *
 * pH midpoint calibration
 */
#pragma once
#include "NumberCollectorState.h"

class PHCalibrationMid : public NumCollectorState {
public:
  PHCalibrationMid(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInCalibration() {
    return true;  // disable controls during calibration
  }
  const char* name() {
    return "PHCalibrationMid";
  }
  float getCurrentValue() {
    return 0.0;
  }
  int getCurrentValuePrecision() {
    return 3;
  }
  const char* prompt() {
    return "pH-Midpoint     ";
  };
  void setValue(double value);
};
