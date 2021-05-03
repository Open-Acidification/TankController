/**
 * PHCalibrationLow.h
 *
 * pH lowpoint calibration
 */
#pragma once
#include "NumberCollectorState.h"

class PHCalibrationLow : public NumCollectorState {
public:
  PHCalibrationLow(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return "PHCalibrationLow";
  }
  float priorValue() {
    return 0.0;
  }
  const char* prompt() {
    return "pH-Lowpoint     ";
  };
  void setValue(double value);
};
