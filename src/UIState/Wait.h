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
  bool isInCalibration() override;
  // watch to see if enough time has passed
  void loop() override;
  const __FlashStringHelper* name() override {
    return F("Wait");
  }
  // override to do nothing
  void start() override {
  }

private:
  // instance variables
  uint32_t endTime = 0;
  UIState* nextState = nullptr;

  // instance methods
  ~Wait();
};
