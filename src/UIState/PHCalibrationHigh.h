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
  const __FlashStringHelper* name() {
    return F("PHCalibrationHigh");
  }
  const char* prompt() {
    return "pH-Highpoint";
  };
  void setValue(float value);
};
