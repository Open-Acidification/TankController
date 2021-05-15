#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "Devices/DateTime_TC.h"
#include "MainMenu.h"
#include "PHCalibrationMid.h"
#include "Serial_TC.h"
#include "TankControllerLib.h"
#include "TemperatureControl.h"
/**
 * These tests test the UpdateControl virtual function for the heater and chiller subclass and
 * checks to see whether the heater or chiller should be in an on state or an off state.
 * The following command lets you skip all the other tests and only run these tests
 * bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=TemperatureControlTest.cpp
 */

const int PIN = 47;

unittest_setup() {
  TankControllerLib* tc = TankControllerLib::instance();
  tc->setNextState(new MainMenu(tc), true);
}

unittest_teardown() {
  TankControllerLib* tc = TankControllerLib::instance();
  tc->setNextState(new MainMenu(tc), true);
}

// Chiller
unittest(BeforeIntervalAndWithinDelta) {
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  chiller.updateControl(20.04);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

// Chiller
unittest(BeforeIntervalAndOutsideDelta) {
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  chiller.updateControl(20.05);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

// Chiller
unittest(AfterIntervalAndWithinDelta) {
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  delay(31000);
  chiller.updateControl(20.04);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

/**
 * Test that is turned on when needed
 * \see unittest(disableChillerDuringCalibration)
 */
unittest(AfterIntervalAndOutsideDelta) {
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
  state->serialPort[0].dataOut = "";  // the history of data written
  // chiller is initially off and goes on when needed
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  delay(31000);
  chiller.updateControl(20.05);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
  assertEqual("2021-01-15 01:48:55\r\nchiller turned on after 31000 ms\r\n", state->serialPort[0].dataOut);
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(31000);
  chiller.updateControl(19.95);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  assertEqual("2021-01-15 01:49:26\r\nchiller turned off after 31000 ms\r\n", state->serialPort[0].dataOut);
}

/**
 * Test that chiller is not turned on during calibration
 * \see unittest(AfterIntervalAndOutsideDelta)
 */
unittest(disableChillerDuringCalibration) {
  TankControllerLib* tc = TankControllerLib::instance();
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  // chiller is initially off and stays off during calibration
  // (test is same as above)
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  delay(31000);
  chiller.updateControl(20.05);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

// Heater
unittest(WithinDelta) {
  GodmodeState* state = GODMODE();
  state->reset();
  Heater heater;
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  heater.setTargetTemperature(20);
  heater.updateControl(19.96);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

/**
 * Test that heater is turned on when needed
 * \see unittest(disableHeaterDuringCalibration)
 */
unittest(OutsideDelta) {
  GodmodeState* state = GODMODE();
  state->reset();
  Heater heater;
  state->serialPort[0].dataOut = "";  // the history of data written
  // heater is initially off, then turns on
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  heater.setTargetTemperature(20);
  heater.updateControl(19.95);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
<<<<<<< HEAD
  assertEqual("2021-01-15 01:48:24\r\nheater turned on after 0 ms\r\n", state->serialPort[0].dataOut);
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(300);
  heater.updateControl(20.05);
  assertEqual("2021-01-15 01:48:24\r\nheater turned off after 300 ms\r\n", state->serialPort[0].dataOut);
=======
  assertEqual("2021-01-15 01:48:24\r\nheater on after 0 ms\r\n", state->serialPort[0].dataOut);
>>>>>>> 9017b881e8e16d68ca35771ee3e130866414b5fa
}

/**
 * Test that heater is not turned on during calibration
 * \see unittest(OutsideDelta)
 */
unittest(disableHeaterDuringCalibration) {
  TankControllerLib* tc = TankControllerLib::instance();
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  GodmodeState* state = GODMODE();
  state->reset();
  Heater heater;
  // heater is initially off, and stays off due to calibration
  // (test is same as above)
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  heater.setTargetTemperature(20);
  heater.updateControl(19.95);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

unittest_main()
