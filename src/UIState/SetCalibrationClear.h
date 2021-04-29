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
    return "Clear pH Calibra";
  };
  void setValue(double value);
};
