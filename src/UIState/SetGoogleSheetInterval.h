/**
 * SetGoogleSheetInterval.h
 *
 * Change Google Sheet Interval
 */
#pragma once
#include "NumberCollector.h"
#include "wrappers/EEPROM_TC.h"

class SetGoogleSheetInterval : public NumberCollector {
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
