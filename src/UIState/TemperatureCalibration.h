/**
 * TemperatureCalibration.h
 *
 * Calibrate the temperature
 */
#pragma once
#include "NumberCollectorState.h"

class TemperatureCalibration : public NumCollectorState {
public:
  bool isInCalibration() {
    return true;
  }
  const __FlashStringHelper* name() {
    return F("TemperatureCalibration");
  }
  float getCurrentValue() {
    return 0;
  }
  uint16_t getCurrentValuePrecision() {
    return 2;
  }
  const __FlashStringHelper* prompt() {
    return F("Real Temperature");
  };
  void setValue(float value);
  bool showCurrentValue() {
    return false;
  }
};
