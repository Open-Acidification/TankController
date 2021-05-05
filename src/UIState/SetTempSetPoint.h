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
  float getCurrentValue() {
    return 0.0;
  }
  int getCurrentValuePrecision() {
    return 2;
  }
  const char* prompt() {
    return "Set Temperature ";
  };
  void setValue(double value);
};
