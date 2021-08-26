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
  SetKP(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return (const char*)F("SetKP");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getKP();
  }
  uint16_t getCurrentValuePrecision() {
    return 1;
  }
  const char* prompt() {
    return (const char*)F("Set KP");
  };
  void setValue(float value);
};
