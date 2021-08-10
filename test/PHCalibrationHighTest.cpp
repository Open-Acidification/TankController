#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Keypad_TC.h"
#include "PHCalibrationHigh.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  PHCalibrationHigh* test = new PHCalibrationHigh(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New High=12.345 ", lines[1]);
  assertEqual("PHCalibrationHigh", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationLow
  tc->loop();  // updateState to PHCalibrationLow
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
