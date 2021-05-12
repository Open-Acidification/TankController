#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "MainMenu.h"
#include "PHCalibrationMid.h"
#include "PHControl.h"
#include "TankControllerLib.h"

const int PIN = 49;

/**
 * cycle the control through to a point of being off
 */
void reset() {
  PHControl* singleton = PHControl::instance();
  singleton->enablePID(false);
  singleton->setTargetPh(7.00);
  singleton->updateControl(7.00);
  delay(10000);
  singleton->updateControl(7.00);
  TankControllerLib* tc = TankControllerLib::instance();
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
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
}

unittest(afterTenSecondsButPhStillHigher) {
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
  delay(1000);
  controlSolenoid->updateControl(7.25);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
}

unittest(afterTenSecondsAndPhIsLower) {
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
  delay(1000);
  controlSolenoid->updateControl(6.75);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

/**
 * Test that CO2 b is turned on when needed
 * \see unittest(disableDuringCalibration)
 */
unittest(beforeTenSecondsButPhIsLower) {
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  // device is initially off but turns on when needed
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PIN]);
  delay(7500);
  controlSolenoid->updateControl(6.75);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

unittest(PhEvenWithTarget) {
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(7.00);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

/**
 * Test that CO2 bubbler is turned on when needed
 * \see unittest(beforeTenSecondsButPhIsLower)
 */
unittest(disableDuringCalibration) {
  TankControllerLib* tc = TankControllerLib::instance();
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  // device is initially off and stays off due to calibration
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PIN]);
}

unittest_main()
