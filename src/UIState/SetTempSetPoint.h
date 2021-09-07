/**
 * SetTempSetPoint.h
 *
 * Set the target Temperature
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"

class SetTempSetPoint : public NumCollectorState {
public:
  SetTempSetPoint(TankController* tc) : NumCollectorState(tc) {
  }
  const __FlashStringHelper* name() {
    return F("SetTempSetPoint");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getTemp();
  }
  uint16_t getCurrentValuePrecision() {
    return 2;
  }
  const __FlashStringHelper* prompt() {
    return F("Set Temperature");
  };
  void setValue(float value);
};
