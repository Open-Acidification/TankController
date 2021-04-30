#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Keypad_TC.h"
#include "SetCalibrationClear.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  SetCalibrationClear* test = new SetCalibrationClear(tc);
  tc->setNextState(test, true);
  test->setValue(2.0);
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Invalid entry!  ", lines[1]);
  assertEqual("SetCalibrationClear", tc->stateName());
  test->setValue(1.0);
  std::vector<String> lines2 = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Cleared!        ", lines2[1]);
  assertEqual("SetCalibrationClear", tc->stateName());
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
