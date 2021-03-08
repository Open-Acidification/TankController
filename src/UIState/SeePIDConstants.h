/**
 * SeePIDConstants.h
 *
 * See PID Constants
 */
#pragma once
#include "UIState.h"

class SeePIDConstants : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "PID Constants";
  };
};
