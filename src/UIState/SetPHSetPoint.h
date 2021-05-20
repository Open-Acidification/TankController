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
  float getCurrentValue();
  uint16_t getCurrentValuePrecision() {
    return 3;
  }
  const char* prompt() {
    return "Set pH Set Point";
  };
  void setValue(float value);
};
