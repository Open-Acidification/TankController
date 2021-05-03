/**
 * SetTankID.h
 *
 * Change Tank ID
 */
#pragma once
#include "NumberCollectorState.h"

class SetTankID : public NumCollectorState {
public:
  SetTankID(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return "SetTankID";
  }
  float getPriorValue() {
    return 0.0;
  }
  const char* prompt() {
    return "Set Tank ID#    ";
  };
  void setValue(double value);
};
