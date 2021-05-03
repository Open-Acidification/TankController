/**
 * SetTempSetPoint.h
 *
 * Set the target Temperature
 */
#pragma once
#include "NumberCollectorState.h"

class SetTempSetPoint : public NumCollectorState {
public:
  SetTempSetPoint(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return "SetTempSetPoint";
  }
  float getPriorValue() {
    return 0.0;
  }
  int getPriorValuePrecision() {
    return 2;
  }
  const char* prompt() {
    return "Set Temperature ";
  };
  void setValue(double value);
};
