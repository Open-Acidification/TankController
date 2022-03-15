/**
 * PHCalibrationLow.h
 *
 * pH lowpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"  // For strnlen

class PHCalibrationLow : public PHCalibration {
public:
  PHCalibrationLow(TankController* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationLow");
  }
  const __FlashStringHelper* prompt() {
    return F("pH-Lowpoint");
  };
  void setValue(float value);
};
