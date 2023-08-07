/**
 * ThermalCalibration.h
 *
 * Calibrate the thermal probe
 */
#pragma once
#include "NumberCollector.h"

class ThermalCalibration : public NumberCollector {
public:
  bool isInCalibration() {
    return true;
  }
  const __FlashStringHelper* name() {
    return F("ThermalCalibration");
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
