#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeeDeviceUptime.h"
#include "TankController.h"
#include "UIState/UIState.h"

unittest(testWaitState) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  GODMODE()->resetClock();
  delay(((((1 * 24 + 2) * 60) + 3) * 60 + 4) * 1000);  // 1 day, 2 hours, 3 minutes, 4 seconds
  SeeDeviceUptime* test = new SeeDeviceUptime();
  tc->setNextState(test, true);  // MainMenu -> SeeDeviceUptime nextState: Wait
  tc->loop(false);
  assertEqual("SeeDeviceUptime", tc->stateName());
  // during the delay we showed the expected two lines
  assertEqual(DateTime_TC::now().as16CharacterString(), display->getLines().at(0).c_str());
  assertEqual("Up d:01 02:03:04", display->getLines().at(1));
  delay(1000);
  tc->loop(false);
  assertEqual(DateTime_TC::now().as16CharacterString(), display->getLines().at(0).c_str());
  assertEqual("Up d:01 02:03:05", display->getLines().at(1));
  delay(1000);
  tc->loop(false);
  assertEqual("Up d:01 02:03:06", display->getLines().at(1));
  delay(1000);
  tc->loop(false);
  assertEqual("Up d:01 02:03:07", display->getLines().at(1));
  delay(1000);
  tc->loop(false);
  assertEqual("Up d:01 02:03:08", display->getLines().at(1));
  delay(1000);
  tc->loop(false);
  assertEqual("Up d:01 02:03:09", display->getLines().at(1));
  delay(1000);
  tc->loop(false);
  assertEqual("Up d:01 02:03:10", display->getLines().at(1));
}

unittest_main()
