#include "SeeDeviceUptime.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"
#include "UIState/UIState.h"

unittest(testWaitState) {
  TankControllerLib* tc = TankControllerLib::instance();
  assertEqual("MainMenu", tc->stateName());
  delay(((((1 * 24 + 2) * 60) + 3) * 60 + 4) * 1000);  // 1 hr 2 minutes
  SeeDeviceUptime* test = new SeeDeviceUptime(tc);
  tc->setNextState(test, true);  // MainMenu -> SeeDeviceUptime nextState: Wait
  assertEqual("SeeDeviceUptime", tc->stateName());
  // during the delay we showed the expected two lines
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual(DateTime_TC::now().as16CharacterString(), lines[0].c_str());
  assertEqual("Up d:01 02:03:04", lines[1]);

  tc->loop();  // SeeDeviceUptime -> Wait
  assertEqual("Wait", tc->stateName());
  delay(900);
  tc->loop();  // Are we done waiting? No!
  assertEqual("Wait", tc->stateName());
  delay(100);
  tc->loop();  // Wait nextState: MainMenu
  tc->loop();  // Wait -> MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
