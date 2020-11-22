#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include <string>
#include <vector>

#include "TankControllerLib.h"

unittest(loop) {
  TankControllerLib* tank = TankControllerLib::instance();
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  assertTrue(tank != nullptr);
  assertTrue(lcd != nullptr);
  assertEqual("LiquidCrystal_CI", lcd->className());
  // get currently displayed lines
  std::vector<std::string> lines = lcd->getLines();
  // verify that display contains 2 empty lines
  assertEqual(2, lines.size());
  assertEqual(16, lines.at(0).length());
  assertEqual(16, lines.at(1).length());
  assertEqual(0, (int)lines.at(0).at(0));
  assertEqual(1, (int)lines.at(0).at(1));
  assertEqual(2, (int)lines.at(0).at(2));
  assertEqual(3, (int)lines.at(0).at(3));
  assertEqual(4, (int)lines.at(1).at(0));
  assertEqual(5, (int)lines.at(1).at(1));
  assertEqual(6, (int)lines.at(1).at(2));
  assertEqual(7, (int)lines.at(1).at(3));
  for (int i = 0; i < lines.size(); ++i) {
    for (int j = 0; j < lines.at(i).size(); ++j) {
      if (lines.at(i).at(j) < ' ') {
        lines.at(i).at(j) = '?';
      }
    }
  }
  assertEqual("???? ONTHANK LAB", lines.at(0));
  assertEqual("????TANK CONTROL", lines.at(1));
}

unittest_main()
