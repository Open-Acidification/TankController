/**
 * SetCalibrationClear.h
 *
 * Clear pH Calibration
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SetCalibrationClear : public UIState {
public:
  SetCalibrationClear(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const char* name() {
    return CSTR("SetCalibrationClear");
  }
  const char* prompt() {
    return CSTR("A: Clear pH Cali");
  };
};
