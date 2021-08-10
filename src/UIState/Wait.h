/**
 * Wait.h
 *
 * Wait and then return to MainMenu
 */
#pragma once
#include "UIState.h"

class Wait : public UIState {
public:
  Wait(TankControllerLib* tc, uint16_t msDelay = 1000, UIState* nextState = nullptr);
  // watch to see if enough time has passed
  void loop();
  const char* name() {
    return "Wait";
  }
  // override to do nothing
  void start() {
  }

private:
  uint32_t endTime = 0;
  UIState* nextState = nullptr;
};
