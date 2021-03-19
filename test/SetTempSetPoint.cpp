#include "SetTempSetPoint.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLibTest tc;
  SetTempSetPoint* test = new SetTempSetPoint(&tc);
  tc.setNextState(test);

  // setValue
  test->setValue(50.05);
  assertEqual(50.05, EEPROM_TC::instance()->getTemp());

  assertTrue(tc.isOnMainMenu());
}

unittest_main()
