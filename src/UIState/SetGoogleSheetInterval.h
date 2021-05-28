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
  SetGoogleSheetInterval(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return "SetGoogleSheetInterval";
  }
  float getCurrentValue() {
    return EEPROM_TC::instance()->getGoogleSheetInterval();
  }
  const char* prompt() {
    return "G Sheet Minutes ";
  };
  void setValue(float value);
};
