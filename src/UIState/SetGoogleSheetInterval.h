/**
 * SetGoogleSheetInterval.h
 *
 * Change Google Sheet Interval
 */
#pragma once
#include "Devices/EEPROM_TC.h"
#include "NumberCollectorState.h"

class SetGoogleSheetInterval : public NumCollectorState {
public:
  virtual bool isInteger() {
    return true;
  }
  const __FlashStringHelper* name() {
    return F("SetGoogleSheetInterval");
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getGoogleSheetInterval();
  }
  const __FlashStringHelper* prompt() {
    return F("G Sheet Minutes");
  };
  void setValue(float value);
};
