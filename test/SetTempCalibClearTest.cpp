#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Keypad_TC.h"
#include "SetTempCalibClear.h"
#include "TankController.h"

// globals for the singletons used in every test
TankController* tc = TankController::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
Keypad* keypad = Keypad_TC::instance()->_getPuppet();

// reduce duplicate code and make it more explicit
void enterKey(char key) {
  keypad->push_back(key);
  tc->loop(false);  // recognize and apply the key entry
}

unittest(test) {
  SetTempCalibClear* test = new SetTempCalibClear(tc);
  tc->setNextState(test, true);
  enterKey('A');
  std::vector<String> lines2 = lc->getLines();
  assertEqual("A: Clear TempCal", lines2[0]);
  assertEqual("Cleared TempCali", lines2[1]);
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // queue MainMenu to be next
  tc->loop(false);  // transition to MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
