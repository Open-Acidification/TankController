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
    return NumCollectorState::isInCalibration();
  }
  const char* name() {
    return "TemperatureCalibration";
  }
  const char* prompt() {
    return "Real Temperature";
  };
  void setValue(double value);
};
