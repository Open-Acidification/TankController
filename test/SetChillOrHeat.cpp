#include "SetChillOrHeat.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

std::vector<String> lines;

void sleepHandler(int millis) {
  lines = LiquidCrystal_TC::instance()->getLines();
}

unittest(default) {
  TankControllerLibTest tc;
  SetChillOrHeat* test = new SetChillOrHeat(&tc);
  tc.setNextState(test);
  assertTrue(EEPROM_TC::instance()->getHeat());
}

unittest(ignoreInvalidValues) {
  TankControllerLibTest tc;
  SetChillOrHeat* test = new SetChillOrHeat(&tc);
  tc.setNextState(test);
  assertTrue(EEPROM_TC::instance()->getHeat());
  test->setValue(2.0);
  assertTrue(EEPROM_TC::instance()->getHeat());
  assertTrue(tc.isOnMainMenu());
}

unittest(switchToHeat) {
  TankControllerLibTest tc;
  SetChillOrHeat* test = new SetChillOrHeat(&tc);
  tc.setNextState(test);
  EEPROM_TC::instance()->setHeat(false);
  test->setValue(9.0);
  assertTrue(EEPROM_TC::instance()->getHeat());
  assertTrue(tc.isOnMainMenu());
}

unittest(switchToChill) {
  TankControllerLibTest tc;
  SetChillOrHeat* test = new SetChillOrHeat(&tc);
  UIState::addSleepHandler(sleepHandler);
  tc.setNextState(test);
  EEPROM_TC::instance()->setHeat(true);
  test->setValue(1.0);
  assertFalse(EEPROM_TC::instance()->getHeat());
  UIState::removeSleepHandler(sleepHandler);
  assertTrue(tc.isOnMainMenu());

  // during the delay we showed the new value
  assertEqual("Use chiller     ", lines[1]);
}

unittest_main()
