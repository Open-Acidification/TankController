/**
 * PHCalibrationPrompt.h
 *
 * Choose 1-point, 2-point, or 3-point pH calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"  // For strnlen

class PHCalibrationPrompt : public PHCalibration {
public:
  PHCalibrationPrompt(TankController* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationPrompt");
  }
  const __FlashStringHelper* prompt() {
    return F("1, 2 or 3 point?");
  };
  uint16_t getCurrentValuePrecision() {
    return 0;
  }
  virtual bool isInteger() {
    return true;
  }
  void setValue(float value);
};
