/**
 * SetKP.h
 *
 * Set the KP for the PID
 */
#pragma once
#include "NumberCollectorState.h"
#include "wrappers/EEPROM_TC.h"

class SetKP : public NumberCollectorState {
public:
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
