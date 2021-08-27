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
  const __FlashStringHelper* prompt() {
    return F("pH-Highpoint");
  };
  void setValue(float value);
};
