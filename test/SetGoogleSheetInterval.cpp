#include "SetGoogleSheetInterval.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

std::vector<String> lines;

void sleepHandler(int millis) {
  lines = LiquidCrystal_TC::instance()->getLines();
}

unittest(test) {
  TankControllerLibTest tc;
  SetGoogleSheetInterval* test = new SetGoogleSheetInterval(&tc);
  UIState::addSleepHandler(sleepHandler);
  tc.setNextState(test);

  // setValue
  test->setValue(30);
  UIState::removeSleepHandler(sleepHandler);
  assertEqual(30, EEPROM_TC::instance()->getGoogleSheetInterval());

  // during the delay we showed the new value
  assertEqual("New interval=30 ", lines[1]);
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
