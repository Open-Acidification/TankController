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
  const __FlashStringHelper* name() {
    return F("SetCalibrationClear");
  }
  const char* prompt() {
    return "A: Clear pH Cali";
  };
};
