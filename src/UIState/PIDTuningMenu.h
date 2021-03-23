/**
 * PIDTuningMenu.h
 *
 * PID Tuning
 */
#pragma once
#include "UIState.h"

class PIDTuningMenu : public UIState {
public:
  PIDTuningMenu(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  String name() {
    return "PIDTuningMenu";
  }
  const char* prompt() {
    return "PID TUNING      ";
  };
};
