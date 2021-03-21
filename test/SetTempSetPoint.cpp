#include "SetTempSetPoint.h"

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
  SetTempSetPoint* test = new SetTempSetPoint(&tc);
  UIState::addSleepHandler(sleepHandler);
  tc.setNextState(test);

  // setValue
  test->setValue(50.05);
  UIState::removeSleepHandler(sleepHandler);
  assertEqual(50.05, EEPROM_TC::instance()->getTemp());

  // during the delay we showed the new value
  assertEqual("New Temp=50.05  ", lines[1]);
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
