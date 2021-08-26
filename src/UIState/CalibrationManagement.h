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
  const char* name() {
    return (const char*)F("CalibrationManagement");
  }
  const char* prompt() {
    return (const char*)F("Cal Management");
  };
};
