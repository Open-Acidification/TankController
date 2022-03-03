/**
 * SetTempCalibClear.h
 *
 * Clear Temperature Calibration
 */
#pragma once
#include "UIState.h"

class SetCalibrationClear : public UIState {
public:
  SetTempCalibClear(TankController* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const __FlashStringHelper* name() {
    return F("SetTempCalibClear");
  }
  const __FlashStringHelper* prompt() {
    return F("A: Clear TempCal");
  };
};
