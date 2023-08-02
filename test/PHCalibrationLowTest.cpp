#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "PHCalibrationLow.h"
#include "TankController.h"

unittest(twoPointLow) {
  TankController* tc = TankController::instance();
  PHCalibrationLower* test = new PHCalibrationLower();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Lower = 12.345  ", lines[1]);
  assertEqual("PHCalibrationLower", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(2000);
  assertTrue(tc->isInCalibration());
  delay(1000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to SeePHCalibration
  tc->loop(false);  // updateState to SeePHCalibration
  assertEqual("SeePHCalibration", tc->stateName());
  assertTrue(tc->isInCalibration());
}

unittest(threePointLow) {
  TankController* tc = TankController::instance();
  PHCalibrationLow* test = new PHCalibrationLow();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Low = 12.345    ", lines[1]);
  assertEqual("PHCalibrationLow", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(2000);
  assertTrue(tc->isInCalibration());
  delay(1000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to SeePHCalibration
  tc->loop(false);  // updateState to SeePHCalibration
  assertEqual("SeePHCalibration", tc->stateName());
  assertTrue(tc->isInCalibration());
}

unittest_main()
