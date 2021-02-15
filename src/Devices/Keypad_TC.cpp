#include "Devices/Keypad_TC.h"

const byte ROWS = 4;
const byte COLS = 4;
char STD_KEYS[ROWS][COLS] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
byte ROW_PINS[ROWS] = {34, 36, 38, 40};
byte COL_PINS[COLS] = {42, 44, 46, 48};

//  class variables
Keypad_TC* Keypad_TC::_instance = nullptr;
Keypad Keypad_TC::puppetPad = Keypad(makeKeymap(STD_KEYS), ROW_PINS, COL_PINS, 4, 4);

Keypad_TC::Keypad_TC() {
}

//  class methods
/**
 * accessor for singleton
 */
Keypad_TC* Keypad_TC::instance() {
  if (!_instance) {
    _instance = new Keypad_TC();
  }
  return _instance;
}

/**
 * Gets a key from the keypad by blocking on key press
 */
char Keypad_TC::waitForKey() {
  lastKey = puppetPad.waitForKey();
  return lastKey;
}

/**
 * Get a key from the keypad if one is queued for use
 */
char Keypad_TC::getKey() {
  lastKey = puppetPad.getKey();
  return lastKey;
}
