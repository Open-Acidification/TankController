#include "SeeDeviceUptime.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"
#include "UIState/UIState.h"

std::vector<String> lines;

void sleepHandler(int millis) {
  lines = LiquidCrystal_TC::instance()->getLines();
}

unittest(test) {
  TankControllerLibTest tc;
  UIState::sleep(((((1 * 24 + 2) * 60) + 3) * 60 + 4) * 1000);  // 1 hr 2 minutes
  UIState::addSleepHandler(sleepHandler);
  SeeDeviceUptime* test = new SeeDeviceUptime(&tc);
  // uptime will be visible for one second
  tc.setNextState(test);
  UIState::removeSleepHandler(sleepHandler);
  // by the time we get back we have returned to the main menu
  assertTrue(tc.isOnMainMenu());

  // during the delay we showed the expected two lines
  assertEqual(DateTime_TC::now().as16CharacterString(), lines[0].c_str());
  assertEqual("Up d:01 02:03:04", lines[1]);
}

unittest_main()
