#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PHCalibrationHigh.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  PHCalibrationHigh* test = new PHCalibrationHigh(tc);
  tc->setNextState(test, true);
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New High=12.345 ", lines[1]);
  assertEqual("PHCalibrationHigh", tc->stateName());
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
