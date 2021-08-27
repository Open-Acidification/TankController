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
  const __FlashStringHelper* name() {
    return F("ResetLCDScreen");
  }
  const __FlashStringHelper* prompt() {
    return F("Clearing Screen");
  };
};
