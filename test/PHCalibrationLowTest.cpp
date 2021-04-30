#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "LiquidCrystal_TC.h"
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
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(1000);
  tc->loop();  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationLow
  tc->loop();  // updateState to PHCalibrationLow
  assertEqual("PHCalibrationHigh", tc->stateName());
}

unittest_main()
