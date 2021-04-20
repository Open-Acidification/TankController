#include "SeeTankID.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(testOutput) {
  // Set up
  int prevID = EEPROM_TC::instance()->getTankID();
  EEPROM_TC::instance()->setTankID(12);

  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeTankID* test = new SeeTankID(tc);
  tc->setNextState(test, true);
  assertEqual("SeeTankID", tc->stateName());

  // Test the output
  assertEqual("Tank ID:        ", display->getLines().at(0));
  assertEqual("12              ", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());

  // Clean up
  EEPROM_TC::instance()->setTankID(prevID);
}

unittest_main()
