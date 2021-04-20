#include "SeeVersion.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(testOutput) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeVersion* test = new SeeVersion(tc);
  tc->setNextState(test, true);
  assertEqual("SeeVersion", tc->stateName());

  // Test the output
  tc->loop();
  assertEqual("Software Version", display->getLines().at(0));
  assertEqual("0.3.0           ", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
