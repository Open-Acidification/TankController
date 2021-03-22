#include "SeeDeviceUptime.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"
#include "UIState/UIState.h"

unittest(testWaitState) {
  TankControllerLibTest tc;
  delay(((((1 * 24 + 2) * 60) + 3) * 60 + 4) * 1000);  // 1 hr 2 minutes
  SeeDeviceUptime* test = new SeeDeviceUptime(&tc);
  tc.setNextState(test);
  // uptime will be visible for one second while we are in Wait state
  assertFalse(tc.isOnMainMenu());
  // during the delay we showed the expected two lines
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual(DateTime_TC::now().as16CharacterString(), lines[0].c_str());
  assertEqual("Up d:01 02:03:04", lines[1]);

  tc.loop();
  assertFalse(tc.isOnMainMenu());
  delay(900);
  tc.loop();
  assertFalse(tc.isOnMainMenu());
  delay(100);
  tc.loop();
  // now we should be back to the main menu
  assertTrue(tc.isOnMainMenu());
}

unittest_main()
