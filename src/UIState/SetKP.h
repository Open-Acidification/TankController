/**
 * SetKP.h
 *
 * Set the KP for the PID
 */
#pragma once
#include "NumberCollectorState.h"

class SetKP : public NumCollectorState {
public:
  SetKP(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return "SetKP";
  }
  float priorValue() {
    return 0.0;
  }
  const char* prompt() {
    return "Set KP          ";
  };
  void setValue(double value);
};
