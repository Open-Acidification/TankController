/**
 * PHCalibrationHigh.h
 *
 * pH highpoint calibration
 */
#pragma once
#include "PHCalibration.h"

class PHCalibrationHigh : public PHCalibration {
public:
  PHCalibrationHigh(TankControllerLib* tc) : PHCalibration(tc) {
  }
  const char* name() {
    return (const char*)F("PHCalibrationHigh");
  }
  const char* prompt() {
    return (const char*)F("pH-Highpoint");
  };
  void setValue(float value);
};
