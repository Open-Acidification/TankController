#include "SetTankID.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TankControllerLib.h"

std::vector<String> lines;

void sleepHandler(int millis) {
  lines = LiquidCrystal_TC::instance()->getLines();
}

unittest(test) {
  TankControllerLibTest tc;
  SetTankID* test = new SetTankID(&tc);
  UIState::addSleepHandler(sleepHandler);
  tc.setNextState(test);

  // setValue
  test->setValue(12);
  UIState::removeSleepHandler(sleepHandler);
  assertEqual(12, EEPROM_TC::instance()->getTankID());

  // during the delay we showed the new value
  assertEqual("Tank ID = 12    ", lines[1]);
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
