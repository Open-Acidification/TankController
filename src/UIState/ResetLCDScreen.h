/**
 * ResetLCDScreen.h
 *
 * Reset LCD Screen
 */
#pragma once
#include "UIState.h"

class ResetLCDScreen : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "Clearing Screen";
  };
};
