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

  assertTrue(tc.isOnMainMenu());
}

unittest_main()
