/**
 * TemperatureCalibration.h
 *
 * Calibrate the temperature
 */
#pragma once
#include "NumberCollectorState.h"

class TemperatureCalibration : public NumCollectorState {
public:
  TemperatureCalibration(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  bool isInCalibration() {
    return true;
  }
  const char* name() {
    return "TemperatureCalibration";
  }
  float getCurrentValue() {
    return 0;
  }
  int getCurrentValuePrecision() {
    return 2;
  }
  const char* prompt() {
    return "Real Temperature";
  };
  void setValue(double value);
};
