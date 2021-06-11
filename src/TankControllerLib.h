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
  static TankControllerLib* instance(const char* pushingBoxID = nullptr);

  // instance methods
  bool isInCalibration();
  void loop();
  void serialEvent();
  void serialEvent1();
  void setNextState(UIState* newState, bool update = false);
  void setup();
  const char* stateName();
  const char* version();

private:
  // class variables
  static TankControllerLib* _instance;
  static const uint32_t IDLE_TIMEOUT = 60L * 1000L;  // revert to the main menu after 60 seconds of inactivity

  // instance variables
  UIState* state = nullptr;
  UIState* nextState = nullptr;
  uint32_t lastKeypadTime = 0;

  // instance methods
  TankControllerLib();
  ~TankControllerLib();
  void blink();
  void handleUI();
  void updateControls();
  void updateState();
  void writeDataToSD();
  void writeDataToSerial();
};
