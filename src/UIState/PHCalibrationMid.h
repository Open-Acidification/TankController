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
  const char* name() {
    return (const char*)F("PHCalibrationMid");
  }
  const char* prompt() {
    return (const char*)F("pH-Midpoint");
  };
  void setValue(float value);
};
