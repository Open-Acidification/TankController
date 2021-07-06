#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <LiquidCrystal_CI.h>
#else
#include <LiquidCrystal.h>
#endif

class LiquidCrystal_TC : public LiquidCrystal {
public:
  static LiquidCrystal_TC* instance(const char *version = "21.01.1");
  void writeLine(const char* text, uint16_t line);

private:
  // class variables
  static LiquidCrystal_TC* _instance;

  // instance methods
  LiquidCrystal_TC(const char *version);
  void splashScreen(const char *version);
};
