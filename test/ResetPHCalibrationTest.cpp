#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "ResetPHCalibration.h"
#include "TankController.h"

// globals for the singletons used in every test
TankController* tc = TankController::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
Keypad* keypad = Keypad_TC::instance()->_getPuppet();

// reduce duplicate code and make it more explicit
void enterKey(char key) {
  keypad->push_back(key);
  tc->loop();  // recognize and apply the key entry
}

unittest(test) {
  ResetPHCalibration* test = new ResetPHCalibration();
  tc->setNextState(test, true);
  assertEqual("A: Clear pH Cali", lc->getLines().at(0));
  enterKey('A');
  tc->loop();
  assertEqual("SeePHCalibration", tc->stateName());
  assertFalse(tc->isInCalibration());
}

unittest_main()
