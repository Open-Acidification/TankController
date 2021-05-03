/**
 * PHCalibrationMid.h
 *
 * pH midpoint calibration
 */
#pragma once
#include "NumberCollectorState.h"

class PHCalibrationMid : public NumCollectorState {
public:
  PHCalibrationMid(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInCalibration() {
    return true;  // disable controls during calibration
  }
  const char* name() {
    return "PHCalibrationMid";
  }
  float priorValue() {
    return 0.0;
  }

  const char* prompt() {
    return "pH-Midpoint     ";
  };
  void setValue(double value);
};
