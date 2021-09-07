/**
 * PHCalibrationMid.h
 *
 * pH midpoint calibration
 */
#pragma once
#include "PHCalibration.h"

class PHCalibrationMid : public PHCalibration {
public:
  PHCalibrationMid(TankController* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationMid");
  }
  const __FlashStringHelper* prompt() {
    return F("pH-Midpoint");
  };
  void setValue(float value);
};
