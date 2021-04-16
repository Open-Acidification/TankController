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
  const char* prompt() {
    return "Set KD          ";
  };
  void setValue(double value);
};
