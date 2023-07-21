#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "DateTime_TC.h"
#include "Devices/DataLogger_TC.h"
#include "LiquidCrystal_TC.h"
#include "MainMenu.h"
#include "PHCalibrationMid.h"
#include "Serial_TC.h"
#include "TankController.h"
#include "TempProbe_TC.h"
#include "TemperatureControl.h"
/**
 * These tests test the UpdateControl virtual function for the heater and chiller subclass and
 * checks to see whether the heater or chiller should be in an on state or an off state.
 * The following command lets you skip all the other tests and only run these tests
 * bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=TemperatureControlTest.cpp
 */

const uint16_t TEMP_CONTROL_PIN = 47;
DataLogger_TC* dataLog = DataLogger_TC::instance();
GodmodeState* state = GODMODE();
TankController* tc = TankController::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
TempProbe_TC* tempProbe = TempProbe_TC::instance();
TemperatureControl* control;

unittest_setup() {
  state->resetClock();
  tempProbe->setTemperature(20, true);
  tc->setNextState(new MainMenu(tc), true);
  state->serialPort[0].dataOut = "";  // the history of data written
}

unittest_teardown() {
  tc->setNextState(new MainMenu(tc), true);
}

// Chiller
unittest(BeforeIntervalAndWithinDelta) {
  TemperatureControl::enableHeater(false);
  control = TemperatureControl::instance();
  control->setTargetTemperature(20);
  control->updateControl(20);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  control->updateControl(20.04);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
}

// Chiller
unittest(BeforeIntervalAndOutsideDelta) {
  TemperatureControl::enableHeater(false);
  control = TemperatureControl::instance();
  control->setTargetTemperature(20);
  control->updateControl(20);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  control->updateControl(20.05);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
}

// Chiller
unittest(AfterIntervalAndWithinDelta) {
  TemperatureControl::enableHeater(false);
  control = TemperatureControl::instance();
  control->setTargetTemperature(20);
  control->updateControl(20);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  delay(31000);
  control->updateControl(20.04);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
}

/**
 * Test that is turned on when needed
 * \see unittest(disableChillerDuringCalibration)
 */
unittest(AfterIntervalAndOutsideDelta) {
  TemperatureControl::enableHeater(false);
  control = TemperatureControl::instance();
  control->setTargetTemperature(20);
  control->updateControl(20);
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
  state->serialPort[0].dataOut = "";  // the history of data written
  // chiller is initially off and goes on when needed
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  assertEqual(0, millis());
  delay(31006);
  assertEqual(31006, millis());
  control->updateControl(20.05);
  assertTrue(control->isOn());
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[TEMP_CONTROL_PIN]);
  assertEqual("chiller turned on at 31006 after 31006 ms\r\n", state->serialPort[0].dataOut);
  tc->loop(false);
  assertEqual("T=20.02 C 20.00 ", lc->getLines().at(1));
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(31012);
  control->updateControl(19.95);
  assertFalse(control->isOn());
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  assertEqual("chiller turned off at 62024 after 31018 ms\r\n", state->serialPort[0].dataOut);
  tc->loop(false);
  assertEqual("T 20.02 c 20.00 ", lc->getLines().at(1));
}

/**
 * Test that chiller is not turned on during calibration
 * \see unittest(AfterIntervalAndOutsideDelta)
 */
unittest(disableChillerDuringCalibration) {
  TemperatureControl::enableHeater(false);
  control = TemperatureControl::instance();
  control->setTargetTemperature(20);
  control->updateControl(20);
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // chiller is initially off and stays off during calibration
  // (test is same as above)
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  delay(31000);
  control->updateControl(20.05);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
}

// Heater
unittest(WithinDelta) {
  TemperatureControl::enableHeater(true);
  control = TemperatureControl::instance();
  control->setTargetTemperature(20);
  control->updateControl(20);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  control->updateControl(19.96);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
}

/**
 * Test that heater is turned on when needed
 * \see unittest(disableHeaterDuringCalibration)
 */
unittest(OutsideDelta) {
  TemperatureControl::enableHeater(true);
  control = TemperatureControl::instance();
  control->setTargetTemperature(20);
  control->updateControl(20);
  // heater is initially off, then turns on
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  control->updateControl(19.95);
  assertTrue(control->isOn());
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[TEMP_CONTROL_PIN]);
  assertEqual("heater turned on at 0 after 0 ms\r\n", state->serialPort[0].dataOut);
  tc->loop(false);
  assertEqual("T 20.00 H 20.00 ", lc->getLines().at(1));
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(300);
  control->updateControl(20.05);
  assertFalse(control->isOn());
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  assertEqual("heater turned off at 306 after 306 ms\r\n", state->serialPort[0].dataOut);
  tc->loop(false);
  assertEqual("T 20.00 h 20.00 ", lc->getLines().at(1));
}

/**
 * Test that heater is not turned on during calibration
 * \see unittest(OutsideDelta)
 */
unittest(disableHeaterDuringCalibration) {
  TemperatureControl::enableHeater(true);
  control = TemperatureControl::instance();
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // heater is initially off, and stays off due to calibration
  // (test is same as above)
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
  control->setTargetTemperature(20);
  control->updateControl(19.95);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_CONTROL_PIN]);
}

unittest(RampGreaterThanZero) {
  float target = 0.0;
  TemperatureControl::enableHeater(false);
  control = TemperatureControl::instance();
  assertFalse(control->isOn());
  control->setTargetTemperature(10);
  control->setRampDuration(1.5);
  assertEqual(TemperatureControl::tempSetTypeTypes::RAMP_TYPE, control->getTempSetType());
  tc->loop(false);
  control->updateControl(tempProbe->getRunningAverage());
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentTemperatureTarget();
  assertTrue(20 <= target && target <= 20.03);
  assertEqual("T 20.00 c 20.00 ", lc->getLines().at(1));
  assertEqual("01/15/2021 01:49:26,   0, 20.02, 20.00, 0.000, 8.100,   62, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  delay(31000);
  // mock arduino restarting
  TemperatureControl::clearInstance();
  control = TemperatureControl::instance();
  // takes 1.5 hours to get to Temp of 7
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentTemperatureTarget();
  assertTrue(16.6 <= target && target <= 16.8);
  assertEqual("T=20.00 C 16.61 ", lc->getLines().at(1));
  assertEqual("01/15/2021 02:18:55,   0, 20.00, 16.61, 0.000, 8.100, 1831, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentTemperatureTarget();
  assertTrue(13.2 <= target && target <= 13.4);
  assertEqual("T=20.00 C 13.28 ", lc->getLines().at(1));
  assertEqual("01/15/2021 02:48:55,   0, 20.00, 13.28, 0.000, 8.100, 3631, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  assertEqual(10, control->getCurrentTemperatureTarget());
  assertEqual("T=20.01 C 10.00 ", lc->getLines().at(1));
  assertEqual("01/15/2021 03:18:55,   0, 20.01, 10.00, 0.000, 8.100, 5431, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  // ramp time no longer used after it ends
  delay(1800000);  // delay 30 minutes
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  assertEqual(10, control->getCurrentTemperatureTarget());
  assertEqual("T=20.01 C 10.00 ", lc->getLines().at(1));
  assertEqual("01/15/2021 04:18:55,   0, 20.01, 10.00, 0.000, 8.100, 9031, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  delay(31000);
  TemperatureControl::enableHeater(true);
  control = TemperatureControl::instance();
  assertFalse(control->isOn());
  control->setTargetTemperature(30);
  control->setRampDuration(1.5);
  control->updateControl(tempProbe->getRunningAverage());
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentTemperatureTarget();
  assertTrue(20 <= target && target <= 20.03);
  assertEqual("T 20.01 h 20.01 ", lc->getLines().at(1));
  assertEqual("01/15/2021 04:19:26,   0, 20.01, 20.01, 0.000, 8.100, 9062, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  // mock arduino restarting
  TemperatureControl::clearInstance();
  control = TemperatureControl::instance();
  // takes 1.5 hours to get to Temp of 7
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentTemperatureTarget();
  assertTrue(23.3 <= target && target <= 23.4);
  assertEqual("T 20.01 H 23.34 ", lc->getLines().at(1));
  assertEqual("01/15/2021 04:49:26,   0, 20.01, 23.34, 0.000, 8.100, 10862, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentTemperatureTarget();
  assertTrue(26.6 <= target && target <= 26.7);
  assertEqual("T 20.01 H 26.67 ", lc->getLines().at(1));
  assertEqual("01/15/2021 05:19:26,   0, 20.01, 26.67, 0.000, 8.100, 12662, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
  // ramp time no longer used after it ends
  delay(1800000);  // delay 30 minutes
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
  assertEqual("T 20.02 H 30.00 ", lc->getLines().at(1));
  assertEqual("01/15/2021 06:49:26,   0, 20.02, 30.00, 0.000, 8.100, 18062, 100000.0,      0.0,      0.0",
              dataLog->buffer);
}

unittest(ChangeRampToZero) {
  TemperatureControl::enableHeater(false);
  control = TemperatureControl::instance();
  assertFalse(control->isOn());
  control->setTargetTemperature(10);
  control->setRampDuration(1.5);
  tc->loop(false);
  assertTrue(20 <= control->getCurrentTemperatureTarget() && control->getCurrentTemperatureTarget() <= 20.03);
  control->setRampDuration(0);
  tc->loop(false);
  assertEqual(10, control->getCurrentTemperatureTarget());
  TemperatureControl::enableHeater(true);
  control = TemperatureControl::instance();
  assertFalse(control->isOn());
  control->setTargetTemperature(30);
  control->setRampDuration(1.5);
  tc->loop(false);
  assertTrue(20 <= control->getCurrentTemperatureTarget() && control->getCurrentTemperatureTarget() <= 20.03);
  control->setRampDuration(0);
  assertEqual(TemperatureControl::tempSetTypeTypes::FLAT_TYPE, control->getTempSetType());
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
}

unittest(sineTest) {
  TemperatureControl::enableHeater(false);
  control = TemperatureControl::instance();
  assertFalse(control->isOn());
  control->setTargetTemperature(10);
  control->setSine(1.5, 2);
  assertEqual(TemperatureControl::tempSetTypeTypes::SINE_TYPE, control->getTempSetType());
  tc->loop(false);
  assertEqual(10, control->getCurrentTemperatureTarget());
  // mock arduino restarting
  TemperatureControl::clearInstance();
  control = TemperatureControl::instance();
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(11.5, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(10, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(8.5, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(10, control->getCurrentTemperatureTarget());
  // make sure sine wave continues
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(11.5, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(10, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(8.5, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(10, control->getCurrentTemperatureTarget());
  TemperatureControl::enableHeater(true);
  control = TemperatureControl::instance();
  assertFalse(control->isOn());
  control->setTargetTemperature(30);
  control->setSine(1.5, 2);
  assertEqual(TemperatureControl::tempSetTypeTypes::SINE_TYPE, control->getTempSetType());
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
  // mock arduino restarting
  TemperatureControl::clearInstance();
  control = TemperatureControl::instance();
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(31.5, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(28.5, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
  // make sure sine wave continues
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(31.5, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(28.5, control->getCurrentTemperatureTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(30, control->getCurrentTemperatureTarget());
}

unittest_main()
