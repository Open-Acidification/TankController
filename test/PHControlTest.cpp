#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "Devices/DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "MainMenu.h"
#include "PHCalibrationMid.h"
#include "PHControl.h"
#include "TankControllerLib.h"

const uint16_t PH_CONTROL_PIN = 49;

GodmodeState* state = GODMODE();
TankControllerLib* tc = TankControllerLib::instance();
PHControl* controlSolenoid = PHControl::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();

/**
 * cycle the control through to a point of being off
 */
void reset() {
  controlSolenoid->enablePID(false);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(7.00);
  delay(10000);
  controlSolenoid->updateControl(7.00);
  tc->setNextState(new MainMenu(tc), true);
}

unittest_setup() {
  reset();
}

unittest_teardown() {
  reset();
}

// updateControl function
unittest(beforeTenSeconds) {
  tc->loop();
  state->resetClock();
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
  delay(1000);
  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setTargetPh(7.00);
  state->serialPort[0].dataOut = "";
  state->serialPort[1].dataIn = "8.00\r";  // the queue of data waiting to be read
  tc->serialEvent1();                      // fake interrupt
  tc->loop();
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  assertEqual("2021-01-15 01:48:25\r\nCO2 bubbler turned on after 1006 ms\r\n", state->serialPort[0].dataOut);
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
}

unittest(afterTenSecondsButPhStillHigher) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(1000);
  controlSolenoid->updateControl(7.25);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
}

unittest(afterTenSecondsAndPhIsLower) {
  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  tc->loop();
  assertEqual("pH=8.000   7.000", lc->getLines().at(0));
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  assertEqual("2021-01-15 01:49:25\r\nCO2 bubbler turned on after 20021 ms\r\n01:49 pH=8.000 temp=-242.02\r\n",
              state->serialPort[0].dataOut);
  tc->loop();
  assertEqual("pH=8.000 B 7.000", lc->getLines().at(0));
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(1000);
  controlSolenoid->updateControl(6.75);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  assertEqual("2021-01-15 01:49:35\r\nCO2 bubbler turned off after 10507 ms\r\n", state->serialPort[0].dataOut);
}

/**
 * Test that CO2 b is turned on when needed
 * \see unittest(disableDuringCalibration)
 */
unittest(beforeTenSecondsButPhIsLower) {
  // device is initially off but turns on when needed
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(7500);
  controlSolenoid->updateControl(6.75);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest(PhEvenWithTarget) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(7.00);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

/**
 * Test that CO2 bubbler is turned on when needed
 * \see unittest(beforeTenSecondsButPhIsLower)
 */
unittest(disableDuringCalibration) {
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // device is initially off and stays off due to calibration
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest_main()
