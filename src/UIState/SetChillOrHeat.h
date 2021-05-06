/**
 * SetChillOrHeat.h
 *
 * Set if the chiller or heater is used
 */
#pragma once
#include "NumberCollectorState.h"

class SetChillOrHeat : public NumCollectorState {
public:
  SetChillOrHeat(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return "SetChillOrHeat";
  }
  float getCurrentValue() {
    return 0.0;
  }
  const char* prompt() {
    return "1:Chill; 9:Heat ";
  };
  void setValue(double value);
};
