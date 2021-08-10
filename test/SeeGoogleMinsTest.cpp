#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeeGoogleMins.h"
#include "TankControllerLib.h"

unittest(testOutput) {
  // Set up
  float prevInterval = EEPROM_TC::instance()->getGoogleSheetInterval();
  EEPROM_TC::instance()->setGoogleSheetInterval(60);

  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeGoogleMins* test = new SeeGoogleMins(tc);
  tc->setNextState(test, true);
  assertEqual("SeeGoogleMins", tc->stateName());

  // Test the output
  assertEqual("Google Mins:    ", display->getLines().at(0));
  assertEqual("60              ", display->getLines().at(1));
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());

  // Clean up
  EEPROM_TC::instance()->setGoogleSheetInterval(prevInterval);
}

unittest_main()
