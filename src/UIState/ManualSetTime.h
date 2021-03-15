/**
 * ManualSetTime.h
 *
 * Manually set the Arduino time
 */
#pragma once
#include "UIState.h"

class ManualSetTime : public UIState {
public:
  ManualSetTime(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const char* prompt() {
    return "Manual Set Time";
  };
};
