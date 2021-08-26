/**
 * PHCalibrationMid.h
 *
 * pH midpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"

class PHCalibrationMid : public PHCalibration {
public:
  PHCalibrationMid(TankControllerLib* tc) : PHCalibration(tc) {
  }
  const char* name() {
    return CSTR("PHCalibrationMid");
  }
  const char* prompt() {
    return CSTR("pH-Midpoint");
  };
  void setValue(float value);
};
