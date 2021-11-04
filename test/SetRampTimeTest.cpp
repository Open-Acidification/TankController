#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "SetRampTime.h"
#include "TankController.h"

unittest(test) {
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  TankController* tc = TankController::instance();
  SetRampTime* test = new SetRampTime(tc);
  tc->setNextState(test, true);

  // get currently displayed lines
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual(16, lines.at(0).length());
  assertEqual(16, lines.at(1).length());
  assertEqual("Set hours:      ", lines.at(0));
  assertEqual("      0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(24);
  lines = lcd->getLines();
  assertEqual("Set minutes:    ", lines.at(0));
  assertEqual("      0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(30);
  lines = lcd->getLines();
  assertEqual("Set seconds:    ", lines.at(0));
  assertEqual("      0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(30);

  // during the delay we showed the new value
  lines = lcd->getLines();
  assertEqual("New Ramp Time:  ", lines[0]);
  assertEqual("24:30:30        ", lines[1]);

  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
