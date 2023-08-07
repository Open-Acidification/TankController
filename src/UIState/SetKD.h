/**
 * SetKD.h
 *
 * Set the KD for the PID
 */
#pragma once
#include "NumberCollectorState.h"
#include "wrappers/EEPROM_TC.h"

class SetKD : public NumberCollectorState {
public:
  const __FlashStringHelper* name() {
    return F("SetKD");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getKD();
  }
  uint16_t getCurrentValuePrecision() {
    return 1;
  }
  const __FlashStringHelper* prompt() {
    return F("Set KD");
  };
  void setValue(float value);
};
