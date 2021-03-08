/**
 * CalibrationManagement.h
 *
 * Calibration Management
 */
#pragma once
#include "UIState.h"

class CalibrationManagement : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "Cal Management";
  };
};
