#include "SeeDeviceUptime.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"
#include "UIState/UIState.h"

unittest(testWaitState) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  delay(((((1 * 24 + 2) * 60) + 3) * 60 + 4) * 1000);  // 1 hr 2 minutes
  SeeDeviceUptime* test = new SeeDeviceUptime(tc);
  tc->setNextState(test, true);  // MainMenu -> SeeDeviceUptime nextState: Wait
  tc->loop();
  assertEqual("SeeDeviceUptime", tc->stateName());
  // during the delay we showed the expected two lines
  assertEqual(DateTime_TC::now().as16CharacterString(), display->getLines().at(0).c_str());
  assertEqual("Up d:01 02:03:04", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual(DateTime_TC::now().as16CharacterString(), display->getLines().at(0).c_str());
  assertEqual("Up d:01 02:03:05", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Up d:01 02:03:06", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Up d:01 02:03:07", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Up d:01 02:03:08", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Up d:01 02:03:09", display->getLines().at(1));
  tc->loop();  // SeeDeviceUptime nextState: MainMenu
  tc->loop();  // SeeDeviceUptime -> MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
