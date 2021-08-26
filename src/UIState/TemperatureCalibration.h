/**
 * TemperatureCalibration.h
 *
 * Calibrate the temperature
 */
#pragma once
#include "NumberCollectorState.h"
#include "TC_util.h"

class TemperatureCalibration : public NumCollectorState {
public:
  TemperatureCalibration(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  bool isInCalibration() {
    return true;
  }
  const char* name() {
    return CSTR("TemperatureCalibration");
  }
  float getCurrentValue() {
    return 0;
  }
  uint16_t getCurrentValuePrecision() {
    return 2;
  }
  const char* prompt() {
    return CSTR("Real Temperature");
  };
  void setValue(float value);
};
