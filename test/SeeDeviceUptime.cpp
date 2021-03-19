#include "SeeDeviceUptime.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLibTest tc;
  SeeDeviceUptime* test = new SeeDeviceUptime(&tc);
  tc.setNextState(test);

  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Up d: 0  0: 0: 0", lines[1]);

  test->handleKey('*');  // any key should do!
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
