/**
 * SetCalibrationClear.h
 *
 * Clear pH Calibration
 */
#pragma once
#include "UIState.h"

class SetCalibrationClear : public UIState {
public:
  SetCalibrationClear(TankControllerLib* tc) : UIState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  void handleKey(char key);
  const char* name() {
    return "SetCalibrationClear";
  }
  float getPriorValue() {
    return 0.0;
  }
  const char* prompt() {
    return "A: Clear pH Cali";
  };
};
