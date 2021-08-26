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
  void handleKey(char key);
  const char* name() {
    return (const char*)F("SetCalibrationClear");
  }
  const char* prompt() {
    return (const char*)F("A: Clear pH Cali");
  };
};
