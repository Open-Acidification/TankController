/**
 * CalibrationManagement.h
 *
 * Calibration Management
 */
#pragma once

#include "TC_util.h"
#include "UIState.h"

class CalibrationManagement : public UIState {
public:
  CalibrationManagement(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const char* name() {
    return CSTR("CalibrationManagement");
  }
  const char* prompt() {
    return CSTR("Cal Management");
  };
};
