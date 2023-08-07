/**
 * SetTankID.h
 *
 * Change Tank ID
 */
#pragma once
#include "NumberCollector.h"
#include "wrappers/EEPROM_TC.h"

class SetTankID : public NumberCollector {
public:
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
