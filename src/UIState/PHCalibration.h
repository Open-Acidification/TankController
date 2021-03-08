/**
 * PHCalibration.h
 *
 * pH One-Point Calibration
 */
#pragma once
#include "UIState.h"

class PHCalibration : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "pH-Calibration";
  };
};
