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
  const char* name() {
    return "SetCalibrationClear";
  }
  const char* prompt() {
    return "1: Clear pH Cali";
  };
  void setValue(double value);
};
