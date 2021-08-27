/**
 * PHCalibrationLow.h
 *
 * pH lowpoint calibration
 */
#pragma once
#include "PHCalibration.h"

class PHCalibrationLow : public PHCalibration {
public:
  PHCalibrationLow(TankControllerLib* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationLow");
  }
  const __FlashStringHelper* prompt() {
    return F("pH-Lowpoint");
  };
  void setValue(float value);
};
