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
  const char* prompt() {
    return "pH-Highpoint    ";
  };
  void setValue(double value);
};
