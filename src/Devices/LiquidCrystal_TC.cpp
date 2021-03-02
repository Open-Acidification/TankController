#include "Devices/LiquidCrystal_TC.h"

// pins used for our LiquidDisplay
const int RS = 24, EN = 22, D4 = 26, D5 = 28, D6 = 30, D7 = 32;

//  class variables
LiquidCrystal_TC* LiquidCrystal_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
LiquidCrystal_TC* LiquidCrystal_TC::instance() {
  if (!_instance) {
    _instance = new LiquidCrystal_TC();
  }
  return _instance;
}

//  instance methods
/**
 * Constructor sets pins, dimensions, and shows splash screen
 */
LiquidCrystal_TC::LiquidCrystal_TC() : LiquidCrystal(RS, EN, D4, D5, D6, D7) {
  begin(16, 2);
  splashScreen();
}

/**
 * Splash screen includes the following logo (actually, this
 * one is double width to get it more nearly proportional):
              XXXXXXXXXXXX
          XXXXXXXXXXXXXXXXXXXX
        XXXXXXXXXXXX  XXXXXXXXXX
      XXXXXX    XXXXXX  XXXXXXXXXX
      XXXXXXXXXX  XXXX  XXXX  XXXX
    XXXXXXXXXXXXXX    XXXXXX  XXXXXX
    XXXXXX    XX  XXXX  XX  XXXXXXXX
    XXXX  XXXX  XXXXXXXX  XXXXXXXXXX
    XXXXXXXXXX  XXXXXXXX  XXXX  XXXX
    XXXXXXXX  XX  XXXX  XX    XXXXXX
    XXXXXX  XXXXXX    XXXXXXXXXXXXXX
      XXXX  XXXX  XXXX  XXXXXXXXXX
      XXXXXXXXXX  XXXXXX    XXXXXX
        XXXXXXXXXX  XXXXXXXXXXXX
          XXXXXXXXXXXXXXXXXXXX
              XXXXXXXXXXXX
*/
void LiquidCrystal_TC::splashScreen() {
  uint8_t OL1[8] = {0b00000, 0b00000, 0b00001, 0b00011, 0b00011, 0b00111, 0b00111, 0b00110};
  uint8_t OL2[8] = {0b00111, 0b11111, 0b11111, 0b10011, 0b11101, 0b11110, 0b00101, 0b11011};
  uint8_t OL3[8] = {0b11100, 0b11111, 0b01111, 0b10111, 0b10110, 0b01110, 0b10101, 0b11011};
  uint8_t OL4[8] = {0b00000, 0b00000, 0b10000, 0b11000, 0b11000, 0b11100, 0b11100, 0b11100};
  uint8_t OL5[8] = {0b00111, 0b00111, 0b00111, 0b00011, 0b00011, 0b00001, 0b00000, 0b00000};
  uint8_t OL6[8] = {0b11011, 0b10101, 0b01110, 0b01101, 0b11101, 0b11110, 0b11111, 0b00111};
  uint8_t OL7[8] = {0b11011, 0b10100, 0b01111, 0b10111, 0b11001, 0b11111, 0b11111, 0b11100};
  uint8_t OL8[8] = {0b01100, 0b11100, 0b11100, 0b11000, 0b11000, 0b10000, 0b00000, 0b00000};
  createChar(0, OL1);
  createChar(1, OL2);
  createChar(2, OL3);
  createChar(3, OL4);
  createChar(4, OL5);
  createChar(5, OL6);
  createChar(6, OL7);
  createChar(7, OL8);
  clear();
  write(uint8_t(0));
  write(uint8_t(1));
  write(uint8_t(2));
  write(uint8_t(3));
  print(F(" ONTHANK LAB"));
  setCursor(0, 1);
  write(uint8_t(4));
  write(uint8_t(5));
  write(uint8_t(6));
  write(uint8_t(7));
  print(F("TANK CONTROL"));
}

/**
 * Prints an input string to the desired line of the LCD screen
 * Even numbers go on the bottom line, odd ones go on the top line
 */
void LiquidCrystal_TC::writeLine(const char* text, int line) {
  line = line % 2;
  setCursor(0, line);
  print("                ");
  setCursor(0, line);
  print(text);
}
