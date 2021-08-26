/**
 * SetKI.h
 *
 * Set the KI for the PID
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"
#include "TC_util.h"

class SetKI : public NumCollectorState {
public:
  SetKI(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* name() {
    return CSTR("SetKI");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getKI();
  }
  uint16_t getCurrentValuePrecision() {
    return 1;
  }
  const char* prompt() {
    return CSTR("Set KI");
  };
  void setValue(float value);
};
