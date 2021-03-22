#include "SetTankID.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLibTest tc;
  SetTankID* test = new SetTankID(&tc);
  tc.setNextState(test);

  // setValue
  test->setValue(12);
  assertEqual(12, EEPROM_TC::instance()->getTankID());

  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Tank ID = 12    ", lines[1]);
  assertFalse(tc.isOnMainMenu());
  delay(1000);
  tc.loop();
  // now we should be back to the main menu
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
