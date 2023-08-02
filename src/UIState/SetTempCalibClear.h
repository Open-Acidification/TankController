/**
 * SetTempCalibClear.h
 *
 * Clear Temperature Calibration
 */
#pragma once
#include "UIState.h"

class SetTempCalibClear : public UIState {
public:
  void handleKey(char key);
  const __FlashStringHelper* name() {
    return F("SetTempCalibClear");
  }
  const __FlashStringHelper* prompt() {
    return F("A: Clear TempCal");
  };
};
