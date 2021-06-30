#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "PHCalibrationMid.h"
#include "TankControllerLib.h"

unittest(test) {
  GodmodeState *state = GODMODE();
  TankControllerLib *tc = TankControllerLib::instance();
  state->reset();
  GODMODE()->serialPort[1].dataIn = "7.125\r";  // the queue of data waiting to be read
  tc->serialEvent1();                           // fake interrupt
  PHProbe *pPHProbe = PHProbe::instance();
  float pH = pPHProbe->getPh();
  assertEqual(7.125, pH);

  std::vector<String> lines;
  PHCalibrationMid *test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.125->     0 ", lines.at(1));
  GODMODE()->serialPort[1].dataIn = "7.325\r";  // the queue of data waiting to be read
  tc->serialEvent1();                           // fake interrupt
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.125->     0 ", lines.at(1));
  tc->loop();
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("  7.325->     0 ", lines.at(1));
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Mid = 12.345", lines.at(1));
  assertEqual("PHCalibrationMid", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationLow
  tc->loop();  // updateState to PHCalibrationLow
  assertEqual("PHCalibrationLow", tc->stateName());
}

unittest_main()
