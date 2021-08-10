#include "Devices/LiquidCrystal_TC.h"

#include "Devices/Serial_TC.h"
#include "TankControllerLib.h"

// useful for future debugging
#include "TC_util.h"

// pins used for our LiquidDisplay
const uint16_t RS = 24, EN = 22, D4 = 26, D5 = 28, D6 = 30, D7 = 32;

//  class variables
LiquidCrystal_TC* LiquidCrystal_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
LiquidCrystal_TC* LiquidCrystal_TC::instance(const char* version) {
  if (!_instance) {
    _instance = new LiquidCrystal_TC(version);
  }
  return _instance;
}

//  instance methods
/**
 * Constructor sets pins, dimensions, and shows splash screen
 */
LiquidCrystal_TC::LiquidCrystal_TC(const char* version) : LiquidCrystal(RS, EN, D4, D5, D6, D7) {
  Serial.println("LiquidCrystal_TC");  // Serial_TC might not be ready yet
  begin(16, 2);
  splashScreen(version);
}

void LiquidCrystal_TC::splashScreen(const char* version) {
  clear();
  print("Tank Controller ");
  setCursor(0, 1);
  print('v');
  print(version);
  print(" loading");
}

/**
 * Prints an input string to the desired line of the LCD screen
 * Even numbers go on the bottom line, odd ones go on the top line
 */
void LiquidCrystal_TC::writeLine(const char* text, uint16_t line) {
  line = line % 2;
  setCursor(0, line);
  char result[17];
  bool moreText = true;
  for (size_t i = 0; i < 16; i++) {
    if (moreText && text[i] == '\0') {
      moreText = false;
    }
    if (moreText) {
      result[i] = text[i];
    } else {
      result[i] = ' ';
    }
  }
  result[16] = '\0';
  print(result);
}
