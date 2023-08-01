#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "PHCalibrationPrompt.h"
#include "TankController.h"

unittest(onePoint) {
  TankController* tc = TankController::instance();
  PHCalibrationPrompt* test = new PHCalibrationPrompt(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(1);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("1, 2 or 3 point?", lines[0]);
  assertEqual("1-pt pH calib...", lines[1]);
  assertEqual("PHCalibrationPrompt", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(1000);
  assertTrue(tc->isInCalibration());
  delay(1000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationOnly
  tc->loop(false);  // updateState to PHCalibrationOnly
  assertEqual("PHCalibrationOnly", tc->stateName());
}

unittest(twoPoint) {
  TankController* tc = TankController::instance();
  PHCalibrationPrompt* test = new PHCalibrationPrompt(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(2);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("1, 2 or 3 point?", lines[0]);
  assertEqual("2-pt pH calib...", lines[1]);
  assertEqual("PHCalibrationPrompt", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(1000);
  assertTrue(tc->isInCalibration());
  delay(1000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationHigher
  tc->loop(false);  // updateState to PHCalibrationHigher
  assertEqual("PHCalibrationHigher", tc->stateName());
}

unittest(threePoint) {
  TankController* tc = TankController::instance();
  PHCalibrationPrompt* test = new PHCalibrationPrompt(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(3);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("1, 2 or 3 point?", lines[0]);
  assertEqual("3-pt pH calib...", lines[1]);
  assertEqual("PHCalibrationPrompt", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(1000);
  assertTrue(tc->isInCalibration());
  delay(1000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationMid
  tc->loop(false);  // updateState to PHCalibrationMid
  assertEqual("PHCalibrationMid", tc->stateName());
}

unittest(badEntry) {
  TankController* tc = TankController::instance();
  PHCalibrationPrompt* test = new PHCalibrationPrompt(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(4);
  // during the delay we showed feedback
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("1, 2 or 3 point?", lines[0]);
  assertEqual("Invalid entry   ", lines[1]);
  assertEqual("PHCalibrationPrompt", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to MainMenu
  tc->loop(false);  // updateState to MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
