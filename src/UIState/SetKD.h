/**
 * SetKD.h
 *
 * Set the KD for the PID
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"

class SetKD : public NumCollectorState {
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
