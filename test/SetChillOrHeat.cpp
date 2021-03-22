#include "SetChillOrHeat.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

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
}

unittest(switchToHeat) {
  TankControllerLibTest tc;
  SetChillOrHeat* test = new SetChillOrHeat(&tc);
  tc.setNextState(test);
  EEPROM_TC::instance()->setHeat(false);
  test->setValue(9.0);
  assertTrue(EEPROM_TC::instance()->getHeat());
}

unittest(switchToChill) {
  TankControllerLibTest tc;
  SetChillOrHeat* test = new SetChillOrHeat(&tc);
  tc.setNextState(test);
  EEPROM_TC::instance()->setHeat(true);
  test->setValue(1.0);
  assertFalse(EEPROM_TC::instance()->getHeat());
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Use chiller     ", lines[1]);
  assertFalse(tc.isOnMainMenu());
  delay(1000);
  tc.loop();
  // now we should be back to the main menu
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
