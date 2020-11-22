/**
 * Abstract superclass for the TankController UI states
 * Concrete subclass for the initial (start) state
 */

#pragma once

#include "../TankControllerLib.h"

class UIState {
public:
  UIState() {
  }
  virtual ~UIState() {
  }
  virtual void handleKey(char key) = 0;

protected:
  void changeState(UIState* state) {
    TankControllerLib::instance()->changeState(state);
  }
};
