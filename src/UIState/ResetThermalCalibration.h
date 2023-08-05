/**
 * ResetThermalCalibration.h
 *
 * Clear Temperature Calibration
 */
#pragma once
#include "UIState.h"

class ResetThermalCalibration : public UIState {
public:
  void handleKey(char key);
  const __FlashStringHelper* name() {
    return F("ResetThermalCalibration");
  }
  const __FlashStringHelper* prompt() {
    return F("A: Clear TempCal");
  };
};
