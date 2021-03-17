#include "SetTankID.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/Keypad_TC.h"
#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLibTest tc;
  SetTankID* test = new SetTankID(&tc);
  tc.setNextState(test);

  // setValue
  test->setValue(12);
  assertEqual(12, EEPROM_TC::instance()->getTankID());

  assertTrue(tc.isOnMainMenu());
}

unittest_main()
