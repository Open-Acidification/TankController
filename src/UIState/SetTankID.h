/**
 * SetTankID.h
 *
 * Change Tank ID
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"
#include "TC_util.h"

class SetTankID : public NumCollectorState {
public:
  SetTankID(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return CSTR("SetTankID");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getTankID();
  }
  const char* prompt() {
    return CSTR("Set Tank ID#");
  };
  void setValue(float value);
};
