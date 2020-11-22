#pragma once
#include <Arduino.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/Serial_TC.h"

const char TANK_CONTROLLER_VERSION[] = "0.3.0";

class UIState;
class TankControllerLib {
public:
  // class methods
  static TankControllerLib* instance();

  // instance methods
  void setup();
  void loop();
  const char* version();

private:
  // class variables
  static TankControllerLib* _instance;

  // instance variables
  UIState* state;
  LiquidCrystal_TC* lcd;
  Serial_TC* log;

  // instance methods
  TankControllerLib();
  ~TankControllerLib();
  void blink();
  void changeState(UIState* newState);

  // other classes with access to our privates
  friend class UIState;
};
