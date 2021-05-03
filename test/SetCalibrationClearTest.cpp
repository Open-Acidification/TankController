#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Keypad_TC.h"
#include "SetCalibrationClear.h"
#include "TankControllerLib.h"

// globals for the singletons used in every test
TankControllerLib* tc = TankControllerLib::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
Keypad* keypad = Keypad_TC::instance()->_getPuppet();

// reduce duplicate code and make it more explicit
void enterKey(char key) {
  keypad->push_back(key);
  tc->loop();  // recognize and apply the key entry
}

unittest(test) {
  SetCalibrationClear* test = new SetCalibrationClear(tc);
  tc->setNextState(test, true);
  enterKey('A');
  std::vector<String> lines2 = lc->getLines();
  assertEqual("Cleared Calibrat", lines2[1]);
  assertEqual("SetCalibrationClear", tc->stateName());
  // Return to mainMenu
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
