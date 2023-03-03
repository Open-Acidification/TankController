#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeeFreeMemory.h"
#include "TankController.h"

unittest(testOutput) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeFreeMemory* test = new SeeFreeMemory(tc);
  tc->setNextState(test, true);
  assertEqual("SeeFreeMemory", tc->stateName());

  // Test the output
  assertEqual("Free Memory:    ", display->getLines().at(0));
  String line2 = display->getLines().at(1);
  assertEqual(0, strcmp("1024 bytes      ", line2.c_str()));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
