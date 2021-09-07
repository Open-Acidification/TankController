/**
 * SetKP.h
 *
 * Set the KP for the PID
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"

class SetKP : public NumCollectorState {
public:
  SetKP(TankController* tc) : NumCollectorState(tc) {
  }
  const __FlashStringHelper* name() {
    return F("SetKP");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getKP();
  }
  uint16_t getCurrentValuePrecision() {
    return 1;
  }
  const __FlashStringHelper* prompt() {
    return F("Set KP");
  };
  void setValue(float value);
};
