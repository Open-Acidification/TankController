#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "PHCalibrationMid.h"
#include "TankController.h"

unittest(onePointMid) {
  GodmodeState *state = GODMODE();
  TankController *tc = TankController::instance();
  state->reset();
  GODMODE()->serialPort[1].dataIn = "7.125\r";  // the queue of data waiting to be read
  tc->serialEvent1();                           // fake interrupt
  PHProbe *pPHProbe = PHProbe::instance();
  float pH = pPHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationMidOne *test = new PHCalibrationMidOne(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.125->     0 ", lines.at(1));
  GODMODE()->serialPort[1].dataIn = "7.325\r";  // the queue of data waiting to be read
  tc->serialEvent1();                           // fake interrupt
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.125->     0 ", lines.at(1));
  tc->loop(false);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.325->     0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Mid = 12.345", lines.at(1));
  assertEqual("PHCalibrationMidOne", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to MainMenu
  tc->loop(false);  // updateState to MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest(twoPointMid) {
  GodmodeState *state = GODMODE();
  TankController *tc = TankController::instance();
  state->reset();
  GODMODE()->serialPort[1].dataIn = "7.125\r";  // the queue of data waiting to be read
  tc->serialEvent1();                           // fake interrupt
  PHProbe *pPHProbe = PHProbe::instance();
  float pH = pPHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationMidTwo *test = new PHCalibrationMidTwo(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.125->     0 ", lines.at(1));
  GODMODE()->serialPort[1].dataIn = "7.325\r";  // the queue of data waiting to be read
  tc->serialEvent1();                           // fake interrupt
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.125->     0 ", lines.at(1));
  tc->loop(false);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.325->     0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Mid = 12.345", lines.at(1));
  assertEqual("PHCalibrationMidTwo", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationLowTwo
  tc->loop(false);  // updateState to PHCalibrationLowTwo
  assertEqual("PHCalibrationLowTwo", tc->stateName());
}

unittest(threePointMid) {
  GodmodeState *state = GODMODE();
  TankController *tc = TankController::instance();
  state->reset();
  GODMODE()->serialPort[1].dataIn = "7.125\r";  // the queue of data waiting to be read
  tc->serialEvent1();                           // fake interrupt
  PHProbe *pPHProbe = PHProbe::instance();
  float pH = pPHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationMidThree *test = new PHCalibrationMidThree(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.125->     0 ", lines.at(1));
  GODMODE()->serialPort[1].dataIn = "7.325\r";  // the queue of data waiting to be read
  tc->serialEvent1();                           // fake interrupt
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.125->     0 ", lines.at(1));
  tc->loop(false);
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.325->     0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Mid = 12.345", lines.at(1));
  assertEqual("PHCalibrationMidThree", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationLowThree
  tc->loop(false);  // updateState to PHCalibrationLowThree
  assertEqual("PHCalibrationLowThree", tc->stateName());
}

unittest_main()
