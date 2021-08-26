/**
 * SetChillOrHeat.h
 *
 * Set if the chiller or heater is used
 */
#pragma once
#include "NumberCollectorState.h"
#include "TC_util.h"

class SetChillOrHeat : public NumCollectorState {
public:
  SetChillOrHeat(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return CSTR("SetChillOrHeat");
  }
  float getCurrentValue();
  const char* prompt() {
    return CSTR("1:Chill; 9:Heat");
  };
  void setValue(float value);
};
