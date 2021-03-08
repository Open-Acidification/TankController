/**
 * TemperatureCalibration.h
 *
 * Calibrate the temperature
 */
#pragma once
#include "UIState.h"

class TemperatureCalibration : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "Temp Calibration";
  };
};
