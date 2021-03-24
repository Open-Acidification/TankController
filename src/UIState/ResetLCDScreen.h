/**
 * ResetLCDScreen.h
 *
 * Reset LCD Screen
 */
#pragma once
#include "UIState.h"

class ResetLCDScreen : public UIState {
public:
  ResetLCDScreen(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const char* name() {
    return "ResetLCDScreen";
  }
  const char* prompt() {
    return "Clearing Screen ";
  };
};
