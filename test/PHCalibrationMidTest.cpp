#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "LiquidCrystal_TC.h"
#include "PHProbe.h"
#include "PHCalibrationMid.h"
#include "TankController.h"

unittest(onePointMid) {
  TankController *tc = TankController::instance();
  PHProbe *pHProbe = PHProbe::instance();
  GODMODE()->reset();
  pHProbe->setPh(7.125);
  float pH = pHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationOnly *test = new PHCalibrationOnly();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  pHProbe->setPh(7.325);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  tc->loop(false);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Buffer = 12.345 ", lines.at(1));
  assertEqual("PHCalibrationOnly", tc->stateName());
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

unittest(twoPointMid) {
  TankController *tc = TankController::instance();
  PHProbe *pHProbe = PHProbe::instance();
  GODMODE()->reset();
  pHProbe->setPh(7.125);
  float pH = pHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationHigher *test = new PHCalibrationHigher();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  pHProbe->setPh(7.325);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  tc->loop(false);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Higher = 12.345 ", lines.at(1));
  assertEqual("PHCalibrationHigher", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(2000);
  assertTrue(tc->isInCalibration());
  delay(1000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationLower
  tc->loop(false);  // updateState to PHCalibrationLower
  assertEqual("PHCalibrationLower", tc->stateName());
  assertTrue(tc->isInCalibration());
}

unittest(threePointMid) {
  TankController *tc = TankController::instance();
  PHProbe *pHProbe = PHProbe::instance();
  GODMODE()->reset();
  pHProbe->setPh(7.125);
  float pH = pHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationMid *test = new PHCalibrationMid();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  pHProbe->setPh(7.325);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  tc->loop(false);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Mid = 12.345    ", lines.at(1));
  assertEqual("PHCalibrationMid", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(2000);
  assertTrue(tc->isInCalibration());
  delay(1000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationHigh
  tc->loop(false);  // updateState to PHCalibrationHigh
  assertEqual("PHCalibrationHigh", tc->stateName());
  assertTrue(tc->isInCalibration());
}

// Test when keys are passed to superclass NumberCollectorState or intercepted
unittest(keyEntry) {
  TankController *tc = TankController::instance();
  PHProbe *pHProbe = PHProbe::instance();
  LiquidCrystal_TC *lc = LiquidCrystal_TC::instance();
  GODMODE()->reset();
  pHProbe->setPh(7.125);
  float pH = pHProbe->getPh();
  assertEqual(7.125, pH);

  PHCalibrationMid *test = new PHCalibrationMid();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  assertEqual("              0 ", lc->getLines().at(1));
  pHProbe->setPh(7.325);
  assertEqual("              0 ", lc->getLines().at(1));
  tc->loop(false);
  assertEqual("              0 ", lc->getLines().at(1));

  // See whether a typed '7' causes the display to update correctly
  test->handleKey('7');
  tc->loop(false);
  assertEqual("              7 ", lc->getLines().at(1));

  // See whether a typed 'D' causes the calibration status to be shown
  test->handleKey('D');
  tc->loop(false);
  assertEqual("SeePHCalibration", tc->stateName());
  assertTrue(tc->isInCalibration());
}

unittest_main()
