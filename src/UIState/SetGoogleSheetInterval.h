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
  String name() {
    return "SetGoogleSheetInterval";
  }
  const char* prompt() {
    return "G Sheet Minutes ";
  };
  void setValue(double value);
};
