/**
 * TemperatureCalibration.h
 *
 * Calibrate the temperature
 */
#pragma once
#include "UIState.h"

class TemperatureCalibration : public UIState {
public:
  TemperatureCalibration(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const char* name() {
    return "TemperatureCalibration";
  }
  const char* prompt() {
    return "Temp Calibration";
  };
};
