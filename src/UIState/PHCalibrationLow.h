/**
 * PHCalibrationLow.h
 *
 * pH lowpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"

class PHCalibrationLow : public PHCalibration {
public:
  PHCalibrationLow(TankControllerLib* tc) : PHCalibration(tc) {
  }
  const char* name() {
    return CSTR("PHCalibrationLow");
  }
  const char* prompt() {
    return CSTR("pH-Lowpoint");
  };
  void setValue(float value);
};
