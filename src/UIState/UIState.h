/**
 * Abstract superclass for the TankController UI states
 * Concrete subclass for the initial (start) state
 */

#pragma once

class TankControllerLib;  // Forward reference
class UIState {
public:
  UIState(TankControllerLib* tc) {
    this->tc = tc;
  }
  virtual void handleKey(char key) = 0;
  virtual bool isMainMenu() {
    return false;
  }
  virtual const char* prompt() = 0;

protected:
  void setNextState(UIState* state);
  void returnToMainMenu();
  TankControllerLib* tc = nullptr;
};
