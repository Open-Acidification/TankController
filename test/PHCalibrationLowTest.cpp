#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "PHCalibrationLow.h"
#include "TankController.h"

unittest(twoPointLow) {
  TankController* tc = TankController::instance();
  PHCalibrationLowTwo* test = new PHCalibrationLowTwo(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Low = 12.345", lines[1]);
  assertEqual("PHCalibrationLowTwo", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to MainMenu
  tc->loop(false);  // updateState to MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest(threePointLow) {
  TankController* tc = TankController::instance();
  PHCalibrationLowThree* test = new PHCalibrationLowThree(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Low = 12.345", lines[1]);
  assertEqual("PHCalibrationLowThree", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationHigh
  tc->loop(false);  // updateState to PHCalibrationHigh
  assertEqual("PHCalibrationHigh", tc->stateName());
}

unittest_main()
