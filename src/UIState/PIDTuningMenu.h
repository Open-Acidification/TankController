/**
 * PIDTuningMenu.h
 *
 * PID Tuning Menu
 */
#pragma once
#include "UIState.h"

class PIDTuningMenu : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "PID TUNING";
  };
};
