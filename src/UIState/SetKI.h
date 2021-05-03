/**
 * SetKI.h
 *
 * Set the KI for the PID
 */
#pragma once
#include "NumberCollectorState.h"

class SetKI : public NumCollectorState {
public:
  SetKI(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return "SetKI";
  }
  float priorValue() {
    return 0.0;
  }
  const char* prompt() {
    return "Set KI          ";
  };
  void setValue(double value);
};
