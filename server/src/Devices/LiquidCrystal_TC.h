#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <LiquidCrystal_CI.h>
#else
#include <LiquidCrystal.h>
#endif

class LiquidCrystal_TC : public LiquidCrystal {
public:
  static LiquidCrystal_TC* instance(const char* version = "?");
  void writeLine(const char* text, uint16_t line);
  void writeLine(const __FlashStringHelper* text, uint16_t line);
  char* getLine(uint16_t line) {
    return _lines[line % 2];
  }

private:
  // class variables
  static LiquidCrystal_TC* _instance;
  char _lines[2][17];

  // instance methods
  LiquidCrystal_TC(const char* version);
  void splashScreen(const char* version);
};
