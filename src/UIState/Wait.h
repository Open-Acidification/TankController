/**
 * Wait.h
 *
 * Wait and then return to MainMenu
 */
#pragma once
#include "UIState.h"

class Wait : public UIState {
public:
  Wait(TankControllerLib* tc, int msDelay = 1000) : UIState(tc) {
    endTime = millis() + msDelay;
  }
  // watch to see if enough time has passed
  void loop();
  // override to do nothing
  void start() {
  }

private:
  uint32_t endTime = 0;
};
