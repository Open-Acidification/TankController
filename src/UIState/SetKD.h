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
  SetKD(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return (const char*)F("SetKD");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getKD();
  }
  uint16_t getCurrentValuePrecision() {
    return 1;
  }
  const char* prompt() {
    return (const char*)F("Set KD");
  };
  void setValue(float value);
};
