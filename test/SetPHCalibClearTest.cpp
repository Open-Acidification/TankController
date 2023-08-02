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
  SetPHCalibClear* test = new SetPHCalibClear();
  tc->setNextState(test, true);
  assertEqual("A: Clear pH Cali", lc->getLines().at(0));
  enterKey('A');
  tc->loop();
  assertEqual("SeePHCalibration", tc->stateName());
  assertFalse(tc->isInCalibration());
}

unittest_main()
