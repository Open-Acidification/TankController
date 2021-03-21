#pragma once
#include <Arduino.h>

#include "TC_assert.h"

const char TANK_CONTROLLER_VERSION[] = "0.3.0";

class LiquidCrystal_TC;
class Serial_TC;
class UIState;
class TankControllerLib {
public:
  // class methods
  static TankControllerLib* instance();

  // instance methods
  void setup();
  void loop();
  const char* version();
  virtual void setNextState(UIState* newState) {
    assert(nextState == nullptr);
    nextState = newState;
  }

protected:
  // class variables
  static TankControllerLib* _instance;

  // instance variables
  UIState* state = nullptr;
  UIState* nextState = nullptr;
  LiquidCrystal_TC* lcd;
  Serial_TC* log;

  // instance methods
  TankControllerLib();
  ~TankControllerLib();
  void blink();
  void handleUI();
  void updateState();
};

#ifdef MOCK_PINS_COUNT
class TankControllerLibTest : public TankControllerLib {
public:
  void setNextState(UIState* newState) {
    assert(nextState == nullptr);
    nextState = newState;
    updateState();
  }
  bool isOnMainMenu();
};
#endif
