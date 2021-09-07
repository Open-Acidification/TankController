#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "SetChillOrHeat.h"
#include "TankController.h"

unittest(ignoreInvalidValues) {
  TankController* tc = TankController::instance();
  SetChillOrHeat* test = new SetChillOrHeat(tc);
  tc->setNextState(test, true);
  assertTrue(EEPROM_TC::instance()->getHeat());
  test->setValue(2.0);
  assertTrue(EEPROM_TC::instance()->getHeat());
  tc->loop();  // transition to Wait
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest(switchToHeat) {
  TankController* tc = TankController::instance();
  SetChillOrHeat* test = new SetChillOrHeat(tc);
  tc->setNextState(test, true);
  EEPROM_TC::instance()->setHeat(false);
  assertEqual(1, test->getCurrentValue());
  test->setValue(9.0);
  assertTrue(EEPROM_TC::instance()->getHeat());
  tc->loop();  // transition to Wait
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest(switchToChill) {
  TankController* tc = TankController::instance();
  SetChillOrHeat* test = new SetChillOrHeat(tc);
  tc->setNextState(test, true);
  EEPROM_TC::instance()->setHeat(true);
  assertEqual(9, test->getCurrentValue());
  test->setValue(1.0);
  assertFalse(EEPROM_TC::instance()->getHeat());
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Use chiller     ", lines[1]);
  assertEqual("SetChillOrHeat", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
