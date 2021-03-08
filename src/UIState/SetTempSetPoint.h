/**
 * SetTempSetPoint.h
 *
 * Set the target Temperature
 */
#pragma once
#include "UIState.h"

class SetTempSetPoint : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "New Temp set";
  };
};
