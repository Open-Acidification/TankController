/**
 * PHCalibrationHigh.h
 *
 * pH highpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"

class PHCalibrationHigh : public PHCalibration {
public:
  PHCalibrationHigh(TankControllerLib* tc) : PHCalibration(tc) {
  }
  const char* name() {
    return CSTR("PHCalibrationHigh");
  }
  const char* prompt() {
    return CSTR("pH-Highpoint");
  };
  void setValue(float value);
};
