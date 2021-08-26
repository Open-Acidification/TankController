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
  const char* name() {
    return (const char*)F("PHCalibrationLow");
  }
  const char* prompt() {
    return (const char*)F("pH-Lowpoint");
  };
  void setValue(float value);
};
