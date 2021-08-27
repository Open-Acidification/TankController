/**
 * SetPHSetPoint.h
 *
 * Set the target pH
 */
#pragma once
#include "NumberCollectorState.h"

class SetPHSetPoint : public NumCollectorState {
public:
  SetPHSetPoint(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const __FlashStringHelper* name() {
    return F("SetPHSetPoint");
  }
  float getCurrentValue();
  uint16_t getCurrentValuePrecision() {
    return 3;
  }
  const __FlashStringHelper* prompt() {
    return F("Set pH Set Point");
  };
  void setValue(float value);
};
