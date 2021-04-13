/**
 * SetKP.h
 *
 * Set the target Temperature
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
  const char* prompt() {
    return "Set KP          ";
  };
  void setValue(double value);
};
