/**
 * SetTempSetPoint.h
 *
 * Set the target Temperature
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"
#include "TC_util.h"

class SetTempSetPoint : public NumCollectorState {
public:
  SetTempSetPoint(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return CSTR("SetTempSetPoint");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getTemp();
  }
  uint16_t getCurrentValuePrecision() {
    return 2;
  }
  const char* prompt() {
    return CSTR("Set Temperature");
  };
  void setValue(float value);
};
