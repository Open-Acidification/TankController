/**
 * PHCalibrationMid.h
 *
 * pH midpoint calibration
 */
#pragma once
#include "PHCalibration.h"

class PHCalibrationMid : public PHCalibration {
public:
  PHCalibrationMid(TankControllerLib* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationMid");
  }
  const char* prompt() {
    return "pH-Midpoint";
  };
  void setValue(float value);
};
