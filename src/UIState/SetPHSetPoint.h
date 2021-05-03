/**
 * SetPHSetPoint.h
 *
 * Set the target pH
 */
#pragma once
#include "NumberCollectorState.h"

class SetPHSetPoint : public NumCollectorState {
public:
  SetPHSetPoint(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return "SetPHSetPoint";
  }
  float getPriorValue() {
    return 0.0;
  }
  int getPriorValuePrecision() {
    return 3;
  }
  const char* prompt() {
    return "Set pH Set Point";
  };
  void setValue(double value);
};
