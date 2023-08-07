/**
 * SetKI.h
 *
 * Set the KI for the PID
 */
#pragma once
#include "NumberCollectorState.h"
#include "wrappers/EEPROM_TC.h"

class SetKI : public NumberCollectorState {
public:
  const __FlashStringHelper* name() {
    return F("SetKI");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getKI();
  }
  uint16_t getCurrentValuePrecision() {
    return 1;
  }
  const __FlashStringHelper* prompt() {
    return F("Set KI");
  };
  void setValue(float value);
};
