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
  const __FlashStringHelper* name() {
    return F("SetChillOrHeat");
  }
  float getCurrentValue();
  const __FlashStringHelper* prompt() {
    return F("1:Chill; 9:Heat");
  };
  void setValue(float value);
};
