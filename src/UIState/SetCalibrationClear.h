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
    return "SetCalibrationClear";
  }
  const char* prompt() {
    return "Clear pH Calibra";
  };
};
