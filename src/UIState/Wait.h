/**
 * Wait.h
 *
 * Wait and then return to MainMenu
 */
#pragma once
#include "UIState.h"

class Wait : public UIState {
public:
  Wait(TankController* tc, uint16_t msDelay = 1000, UIState* nextState = nullptr);
  bool isInCalibration() {
    return nextState->isInCalibration();
  }
  // watch to see if enough time has passed
  void loop();
  const __FlashStringHelper* name() {
    return F("Wait");
  }
  // override to do nothing
  void start() {
  }

private:
  uint32_t endTime = 0;
  UIState* nextState = nullptr;
};
