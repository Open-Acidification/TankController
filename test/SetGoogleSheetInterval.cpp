#include "SetGoogleSheetInterval.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLibTest tc;
  SetGoogleSheetInterval* test = new SetGoogleSheetInterval(&tc);
  tc.setNextState(test);

  // setValue
  test->setValue(30);
  assertEqual(30, EEPROM_TC::instance()->getGoogleSheetInterval());

  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New interval=30 ", lines[1]);
  assertFalse(tc.isOnMainMenu());
  delay(1000);
  tc.loop();
  // now we should be back to the main menu
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
