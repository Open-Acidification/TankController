/**
 * ManualSetTime.h
 *
 * Manually set the Arduino time
 */
#pragma once
#include "UIState.h"

class ManualSetTime : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "Manual Set Time";
  };
};
