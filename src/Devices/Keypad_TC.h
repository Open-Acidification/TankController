#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <Keypad_CI.h>
#else
#include <Keypad.h>
#endif

class Keypad_TC {
public:
  // class methods
  static Keypad_TC* instance();

  // Public
  char waitForKey();
  char getKey();

  // Testing support
  static Keypad* _getPuppet() {
    return &puppetPad;
  }

private:
  // class
  static Keypad_TC* _instance;
  static Keypad puppetPad;
  Keypad_TC();

  // instance
  char lastKey;
};
