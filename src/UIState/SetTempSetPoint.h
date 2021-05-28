/**
 * SetTempSetPoint.h
 *
 * Set the target Temperature
 */
#pragma once
#include "NumberCollectorState.h"

#include "EEPROM_TC.h"

class SetTempSetPoint : public NumCollectorState {
public:
  SetTempSetPoint(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return "SetTempSetPoint";
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getTemp();
  }
  uint16_t getCurrentValuePrecision() {
    return 2;
  }
  const char* prompt() {
    return "Set Temperature ";
  };
  void setValue(float value);
};
