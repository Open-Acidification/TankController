#include "SetPHSetPoint.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLibTest tc;
  SetPHSetPoint* test = new SetPHSetPoint(&tc);
  tc.setNextState(test);

  // setValue
  test->setValue(7.1234);
  assertEqual(7.1234, EEPROM_TC::instance()->getPH());

  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New pH=7.1234   ", lines[1]);
  assertFalse(tc.isOnMainMenu());
  delay(1000);
  tc.loop();
  // now we should be back to the main menu
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
