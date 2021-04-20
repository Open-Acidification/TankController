#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "PHCalibrationLow.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  PHCalibrationLow* test = new PHCalibrationLow(tc);
  tc->setNextState(test, true);
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Low = 12.345", lines[1]);
  assertEqual("PHCalibrationLow", tc->stateName());
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
