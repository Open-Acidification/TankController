#include "SetPHSetPoint.h"

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
  SetPHSetPoint* test = new SetPHSetPoint(&tc);
  UIState::addSleepHandler(sleepHandler);
  tc.setNextState(test);

  // setValue
  test->setValue(7.1234);
  UIState::removeSleepHandler(sleepHandler);
  assertEqual(7.1234, EEPROM_TC::instance()->getPH());

  // during the delay we showed the new value
  assertEqual("New pH=7.1234   ", lines[1]);
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
