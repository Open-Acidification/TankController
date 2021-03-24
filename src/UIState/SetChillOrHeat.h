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
  const char* name() {
    return "SetChillOrHeat";
  }
  const char* prompt() {
    return "1:Chill; 9:Heat ";
  };
  void setValue(double value);
};
