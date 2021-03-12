#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/Keypad_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  lines = lc->getLines();
  assertEqual(" ONTHANK LAB", lc->getLines().at(0).substr(4));
  keypad->push_back('A');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Set pH Set Point", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest_main()
