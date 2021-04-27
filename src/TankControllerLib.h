#pragma once
#include <Arduino.h>

#include "TC_util.h"

class LiquidCrystal_TC;
class Serial_TC;
class UIState;
class TankControllerLib {
public:
  // class methods
  static TankControllerLib* instance();

  // instance methods
  void loop();
  void serialEvent();
  void serialEvent1();
  void setCalibrationMode(bool flag);
  void setNextState(UIState* newState, bool update = false);
  void setup();
  const char* stateName();
  const char* version();
  void writeDataToSD();

private:
  // class variables
  static TankControllerLib* _instance;
  static const int IDLE_TIMEOUT = 60 * 1000;  // revert to the main menu after 60 seconds of inactivity

  // instance variables
  bool calibrationMode = false;
  UIState* state = nullptr;
  UIState* nextState = nullptr;
  LiquidCrystal_TC* lcd;
  Serial_TC* log;
  unsigned long lastKeypadTime = 0;

  // instance methods
  TankControllerLib();
  ~TankControllerLib();
  void blink();
  void handleUI();
  void updateControls();
  void updateState();
};
