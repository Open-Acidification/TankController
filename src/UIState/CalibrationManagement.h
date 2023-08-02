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
  const __FlashStringHelper* name() {
    return F("CalibrationManagement");
  }
  const __FlashStringHelper* prompt() {
    return F("Cal Management");
  };
};
