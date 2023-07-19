#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Keypad_TC.h"
#include "SetPHCalibClear.h"
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
  SetPHCalibClear* test = new SetPHCalibClear(tc);
  tc->setNextState(test, true);
  enterKey('A');
  std::vector<String> lines2 = lc->getLines();
  assertEqual("A: Clear pH Cali", lines2[0]);
  assertEqual("SeePHCalibration", tc->stateName());
}

unittest_main()
