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
  // virtual so subclass destructor is called
  virtual ~UIState() {
  }
  virtual void handleKey(char key);
  virtual bool isMainMenu() {
    return false;
  }
  virtual const char* prompt() {
    return "                ";
  }
  virtual void start();

protected:
  void setNextState(UIState* state);
  void returnToMainMenu(int msDelay = 0);
  TankControllerLib* tc = nullptr;
};
