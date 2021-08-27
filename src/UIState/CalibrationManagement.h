/**
 * CalibrationManagement.h
 *
 * Calibration Management
 */
#pragma once
#include "UIState.h"

class CalibrationManagement : public UIState {
public:
  CalibrationManagement(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const __FlashStringHelper* name() {
    return F("CalibrationManagement");
  }
  const char* prompt() {
    return "Cal Management";
  };
};
