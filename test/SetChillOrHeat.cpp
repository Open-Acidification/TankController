#include "SetChillOrHeat.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

unittest(ignoreInvalidValues) {
  TankControllerLib* tc = TankControllerLib::instance();
  SetChillOrHeat* test = new SetChillOrHeat(tc);
  tc->setNextState(test, true);
  assertTrue(EEPROM_TC::instance()->getHeat());
  test->setValue(2.0);
  assertTrue(EEPROM_TC::instance()->getHeat());
  tc->loop();  // transition to Wait
  delay(1000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest(switchToHeat) {
  TankControllerLib* tc = TankControllerLib::instance();
  SetChillOrHeat* test = new SetChillOrHeat(tc);
  tc->setNextState(test, true);
  EEPROM_TC::instance()->setHeat(false);
  test->setValue(9.0);
  assertTrue(EEPROM_TC::instance()->getHeat());
  tc->loop();  // transition to Wait
  delay(1000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest(switchToChill) {
  TankControllerLib* tc = TankControllerLib::instance();
  SetChillOrHeat* test = new SetChillOrHeat(tc);
  tc->setNextState(test, true);
  EEPROM_TC::instance()->setHeat(true);
  test->setValue(1.0);
  assertFalse(EEPROM_TC::instance()->getHeat());
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Use chiller     ", lines[1]);
  assertEqual("SetChillOrHeat", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(1000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
