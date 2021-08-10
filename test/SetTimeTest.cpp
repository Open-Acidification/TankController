#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "SetTime.h"
#include "TankControllerLib.h"

unittest(test) {
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  TankControllerLib* tc = TankControllerLib::instance();
  SetTime* test = new SetTime(tc);
  tc->setNextState(test, true);
  DateTime_TC june(2021, 06, 01, 20, 57, 15);
  june.setAsCurrent();

  DateTime_TC now = DateTime_TC::now();
  // the default time is the code compile time
  assertTrue(now.year() == 2021);

  // get currently displayed lines
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual(16, lines.at(0).length());
  assertEqual(16, lines.at(1).length());
  assertEqual("Set Year (YYYY):", lines.at(0));
  assertEqual("   2021->     0 ", lines.at(1));
  assertEqual(2021, test->getCurrentValue());
  test->setValue(2020);
  lines = lcd->getLines();
  assertEqual("Month (1-12):   ", lines.at(0));
  assertEqual("      6->     0 ", lines.at(1));
  assertEqual(06, test->getCurrentValue());
  test->setValue(03);
  lines = lcd->getLines();
  assertEqual("Day (1-31):     ", lines.at(0));
  assertEqual("      1->     0 ", lines.at(1));
  assertEqual(01, test->getCurrentValue());
  test->setValue(18);
  lines = lcd->getLines();
  assertEqual("Hour (0-23):    ", lines.at(0));
  assertEqual("     20->     0 ", lines.at(1));
  assertEqual(20, test->getCurrentValue());
  test->setValue(13);
  lines = lcd->getLines();
  assertEqual("Minute (0-59):  ", lines.at(0));
  assertEqual("     57->     0 ", lines.at(1));
  assertEqual(57, test->getCurrentValue());
  test->setValue(15);

  // during the delay we showed the new value
  lines = lcd->getLines();
  assertEqual("New Date/Time:  ", lines[0]);
  assertEqual("2020-03-18 13:15", lines[1]);

  // a year ago ensures that it precedes the compile time
  assertEqual("2020-03-18 13:15", DateTime_TC::now().as16CharacterString());
  assertEqual("SetTime", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
