#pragma once
#include <Arduino.h>

#include "TC_util.h"

#define TURN_SOLENOID_OFF HIGH
#define TURN_SOLENOID_ON LOW

class LiquidCrystal_TC;
class Serial_TC;
class UIState;
class TankControllerLib {
public:
  // class methods
  static TankControllerLib* instance();

  // instance methods
  bool isInCalibration();
  void loop();
  void serialEvent();
  void serialEvent1();
  void setNextState(UIState* newState, bool update = false);
  void setup();
  const char* stateName();
  const char* version();
  void writeDataToSD();

private:
  // class variables
  static TankControllerLib* _instance;
  static const unsigned long IDLE_TIMEOUT = 60L * 1000L;  // revert to the main menu after 60 seconds of inactivity

  // instance variables
  UIState* state = nullptr;
  UIState* nextState = nullptr;
  unsigned long lastKeypadTime = 0;

  // instance methods
  TankControllerLib();
  ~TankControllerLib();
  void blink();
  void handleUI();
  void updateControls();
  void updateState();
};
