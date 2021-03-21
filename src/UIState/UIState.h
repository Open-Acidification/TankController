/**
 * Abstract superclass for the TankController UI states
 * Concrete subclass for the initial (start) state
 */

#pragma once

#include <Arduino.h>

typedef void (*SleepHandler)(int ms);

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

  static bool addSleepHandler(SleepHandler pFunction);
  static bool removeSleepHandler(SleepHandler pFunction);
  static void sleep(int ms);

  // instance methods
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

private:
  static const int SLEEP_HANDLERS_COUNT = 5;
  static SleepHandler sleepHandlers[SLEEP_HANDLERS_COUNT];
};
