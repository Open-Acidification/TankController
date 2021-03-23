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
  String name() {
    return "SetTempSetPoint";
  }
  const char* prompt() {
    return "Set Temperature ";
  };
  void setValue(double value);
};
