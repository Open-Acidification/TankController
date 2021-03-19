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

  assertTrue(tc.isOnMainMenu());
}

unittest_main()
