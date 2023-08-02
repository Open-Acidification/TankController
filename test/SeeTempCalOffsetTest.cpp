#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeeTempCalOffset.h"
#include "TankController.h"

unittest(testOutput) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeTempCalOffset* test = new SeeTempCalOffset();
  tc->setNextState(test, true);
  assertEqual("SeeTempCalOffset", tc->stateName());

  // Test the prompt
  assertEqual("Temp Cal Offset:", display->getLines().at(0));
  // Check value
  assertEqual("0.00000         ", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
