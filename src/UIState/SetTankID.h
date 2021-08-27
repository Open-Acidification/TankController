/**
 * SetTankID.h
 *
 * Change Tank ID
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"

class SetTankID : public NumCollectorState {
public:
  SetTankID(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const __FlashStringHelper* name() {
    return F("SetTankID");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getTankID();
  }
  const __FlashStringHelper* prompt() {
    return F("Set Tank ID#");
  };
  void setValue(float value);
};
