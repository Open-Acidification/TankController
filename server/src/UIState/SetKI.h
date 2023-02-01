/**
 * SetKI.h
 *
 * Set the KI for the PID
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"

class SetKI : public NumCollectorState {
public:
  SetKI(TankController* tc) : NumCollectorState(tc) {
  }
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
