#include "SeeTankID.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(testOutput) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeTankID* test = new SeeTankID(tc);
  tc->setNextState(test, true);
  assertEqual("SeeTankID", tc->stateName());

  // Test the output
  EEPROM_TC::instance()->setTankID(1);
  tc->loop();
  assertEqual("Tank ID:        ", display->getLines().at(0));
  assertEqual("1               ", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
