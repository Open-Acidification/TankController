/**
 * SetKD.h
 *
 * Set the KD for the PID
 */
#pragma once
#include "NumberCollectorState.h"

class SetKD : public NumCollectorState {
public:
  SetKD(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return "SetKD";
  }
  float getCurrentValue() {
    return 0.0;
  }
  uint16_t getCurrentValuePrecision() {
    return 1;
  }
  const char* prompt() {
    return "Set KD          ";
  };
  void setValue(float value);
};
