/**
 * ResetPHCalibration.h
 *
 * Clear pH Calibration
 */
#pragma once
#include "UIState.h"

class ResetPHCalibration : public UIState {
public:
  void handleKey(char key);
  const __FlashStringHelper* name() {
    return F("ResetPHCalibration");
  }
  const __FlashStringHelper* prompt() {
    return F("A: Clear pH Cali");
  };
};
