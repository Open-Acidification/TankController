/**
 * SetGoogleSheetInterval.h
 *
 * Change Google Sheet Interval
 */
#pragma once
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
    return 0.0;
  }
  const char* prompt() {
    return "G Sheet Minutes ";
  };
  void setValue(double value);
};
