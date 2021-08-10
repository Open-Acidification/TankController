#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "SetGoogleSheetInterval.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  SetGoogleSheetInterval* test = new SetGoogleSheetInterval(tc);
  tc->setNextState(test, true);

  // getCurrentValue (default)
  assertEqual(65535, test->getCurrentValue());

  // setValue
  test->setValue(30);
  assertEqual(30, EEPROM_TC::instance()->getGoogleSheetInterval());
  // getCurrentValue (modified)
  assertEqual(30, test->getCurrentValue());

  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New interval=30 ", lines[1]);
  assertEqual("SetGoogleSheetInterval", tc->stateName());

  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
