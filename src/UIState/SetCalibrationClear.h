/**
 * SetCalibrationClear.h
 *
 * Clear pH Calibration
 */
#pragma once
#include "NumberCollectorState.h"

class SetCalibrationClear : public NumCollectorState {
public:
  SetCalibrationClear(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return "SetCalibrationClear";
  }
  float priorValue() {
    return 0.0;
  }
  const char* prompt() {
    return "1: Clear pH Cali";
  };
  void setValue(double value);
};
