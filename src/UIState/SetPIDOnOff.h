/**
 * SetPIDOnOff.h
 *
 * Turn PID on/off
 */
#pragma once
#include "UIState.h"

class SetPIDOnOff : public UIState {
public:
  SetPIDOnOff(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const char* name() {
    return "SetPIDOnOff";
  }
  const char* prompt() {
    return "PID TUNING      ";
  };
};
