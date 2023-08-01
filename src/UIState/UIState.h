/**
 * Abstract superclass for the TankController UI states
 * Concrete subclass for the initial (start) state
 */

#pragma once

#include <Arduino.h>

class TankController;  // Forward reference
class UIState {
public:
  // constructor that captures the owning TankController
  UIState(TankController* = nullptr);
  // virtual destructor so subclass destructor is called
  virtual ~UIState() {
  }

  // instance methods
  virtual void handleKey(char key);
  virtual bool isInCalibration() {
    return false;  // override to return true to disable controls
  }
  virtual bool isMainMenu() {
    return false;
  }
  virtual void loop() {
  }
  virtual const __FlashStringHelper* name() = 0;
  virtual const __FlashStringHelper* prompt() {
    return F("");
  }
  virtual void start();

protected:
  void setNextState(UIState* state);
  void returnToMainMenu(uint16_t msDelay = 0);
  TankController* tc = nullptr;
};
