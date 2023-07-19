#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationMid.h"
#include "TankController.h"

unittest(onePointMid) {
  TankController *tc = TankController::instance();
  PHProbe *pPHProbe = PHProbe::instance();
  GODMODE()->reset();
  pPHProbe->setPh(7.125);
  float pH = pPHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationMid *test = new PHCalibrationMid(tc, 1);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  pPHProbe->setPh(7.325);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  tc->loop(false);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Mid = 12.345", lines.at(1));
  assertEqual("PHCalibrationMid", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to SeePHCalibration
  tc->loop(false);  // updateState to SeePHCalibration
  assertEqual("SeePHCalibration", tc->stateName());
  assertTrue(tc->isInCalibration());
}

unittest(twoPointMid) {
  TankController *tc = TankController::instance();
  PHProbe *pPHProbe = PHProbe::instance();
  GODMODE()->reset();
  pPHProbe->setPh(7.125);
  float pH = pPHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationMid *test = new PHCalibrationMid(tc, 2);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  pPHProbe->setPh(7.325);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  tc->loop(false);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("              0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Mid = 12.345", lines.at(1));
  assertEqual("PHCalibrationMid", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationLow
  tc->loop(false);  // updateState to PHCalibrationLow
  assertEqual("PHCalibrationLow", tc->stateName());
  assertTrue(tc->isInCalibration());
}

unittest_main()
