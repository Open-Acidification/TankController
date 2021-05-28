/**
 * SetTankID.h
 *
 * Change Tank ID
 */
#pragma once
#include "NumberCollectorState.h"

#include "Devices/EEPROM_TC.h"

class SetTankID : public NumCollectorState {
public:
  SetTankID(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return "SetTankID";
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getTankID();
  }
  const char* prompt() {
    return "Set Tank ID#    ";
  };
  void setValue(float value);
};
