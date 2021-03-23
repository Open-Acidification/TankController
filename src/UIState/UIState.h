/**
 * Abstract superclass for the TankController UI states
 * Concrete subclass for the initial (start) state
 */

#pragma once

#include <Arduino.h>

class TankControllerLib;  // Forward reference
class UIState {
public:
  // constructor that captures the owning TankControllerLib
  UIState(TankControllerLib* tc) {
    this->tc = tc;
  }
  // virtual destructor so subclass destructor is called
  virtual ~UIState() {
  }

  // instance methods
  virtual void handleKey(char key);
  virtual bool isMainMenu() {
    return false;
  }
  virtual void loop() {
  }
  virtual String name() = 0;
  virtual const char* prompt() {
    return "                ";
  }
  virtual void start();

protected:
  void setNextState(UIState* state);
  void returnToMainMenu(int msDelay = 0);
  TankControllerLib* tc = nullptr;
};
