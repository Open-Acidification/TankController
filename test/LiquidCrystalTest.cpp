#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include <vector>

#include "Devices/LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(loop) {
  TankControllerLib* tank = TankControllerLib::instance();
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  assertTrue(tank != nullptr);
  assertTrue(lcd != nullptr);
  assertEqual("LiquidCrystal_CI", lcd->className());
  // get currently displayed lines
  std::vector<String> lines = lcd->getLines();
  // verify that display contains the splash screen
  assertEqual(2, lines.size());
  assertEqual(16, lines.at(0).length());
  assertEqual(16, lines.at(1).length());
  assertEqual("Tank Controller ", lines.at(0));
  assertEqual(
      "v"
      "21.07.4 loading",  // this allows a word-search to find the number
      lines.at(1));
}

unittest(writeLine) {
  LiquidCrystal_TC* testLcd = LiquidCrystal_TC::instance();
  testLcd->writeLine("Hello", 0);
  testLcd->writeLine("World", 1);
  std::vector<String> lines = testLcd->getLines();
  assertEqual("Hello           ", lines.at(0));
  assertEqual("World           ", lines.at(1));

  testLcd->writeLine("Wrong line num", 6);
  testLcd->writeLine("this won't break", 3);
  lines = testLcd->getLines();
  assertEqual("Wrong line num  ", lines.at(0));
  assertEqual("this won't break", lines.at(1));
}

unittest_main()
