#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "../src/Devices/Keypad_TC.h"
#include "../src/Devices/LiquidCrystal_TC.h"
#include "../src/TankController.h"
#include "../src/UIState/SeeVersion.h"

unittest(testOutput) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeVersion* test = new SeeVersion(tc);
  tc->setNextState(test, true);
  assertEqual("SeeVersion", tc->stateName());

  // Test the output
  tc->loop(false);
  assertEqual("Software Version", display->getLines().at(0));
  assertEqual("23.7.0         ", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
