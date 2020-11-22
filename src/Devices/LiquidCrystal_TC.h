#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <LiquidCrystal_CI.h>
#else
#include <LiquidCrystal.h>
#endif

class LiquidCrystal_TC : public LiquidCrystal {
public:
  // class methods
  static LiquidCrystal_TC* instance();

private:
  // class variables
  static LiquidCrystal_TC* _instance;

  // instance methods
  LiquidCrystal_TC();
  void splashScreen();
};
