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
    return "PHCalibrationLow";
  }
  const char* prompt() {
    return "pH-Lowpoint     ";
  };
  void setValue(float value);
};
