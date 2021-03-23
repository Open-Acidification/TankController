/**
 * SeePIDConstants.h
 *
 * See PID Constants
 */
#pragma once
#include "UIState.h"

class SeePIDConstants : public UIState {
public:
  SeePIDConstants(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  String name() {
    return "SeePIDConstants";
  }
  const char* prompt() {
    return "PID Constants   ";
  };
};
