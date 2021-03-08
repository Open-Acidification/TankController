#include "SetTempSetPoint.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include <iostream>

#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  SetTempSetPoint test;
  // setValue
  test.setValue(50.05);
  assertEqual(50.05, EEPROM_TC::instance()->getTemp());
}

// 0.000=4 digits
unittest(Number_Digits) {
  SetTempSetPoint test;
  assertEqual(4, test.getExpectedDigits());
}

unittest(Next_State) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();

  keypad->push_back('B');
  tc->loop();
  assertEqual("Set Temperature ", lc->getLines().at(0));

  keypad->push_back('A');
  tc->loop();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest_main()
