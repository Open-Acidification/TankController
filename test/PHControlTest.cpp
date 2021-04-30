#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "PHControl.h"

const int PIN = 49;

/**
 * cycle the control through to a point of being off
 */
void reset() {
  PHControl* singleton = PHControl::instance();
  singleton->setUsePID(false);
  singleton->setTargetPh(7.00);
  singleton->updateControl(7.00);
  delay(10000);
  singleton->updateControl(7.00);
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
  assertEqual(HIGH, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(LOW, state->digitalPin[PIN]);
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(LOW, state->digitalPin[PIN]);
}

unittest(afterTenSecondsButPhStillHigher) {
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  assertEqual(HIGH, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(LOW, state->digitalPin[PIN]);
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(LOW, state->digitalPin[PIN]);
  delay(1000);
  controlSolenoid->updateControl(7.25);
  assertEqual(LOW, state->digitalPin[PIN]);
}

unittest(afterTenSecondsAndPhIsLower) {
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  assertEqual(HIGH, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(LOW, state->digitalPin[PIN]);
  delay(9500);
  controlSolenoid->updateControl(8.00);
  assertEqual(LOW, state->digitalPin[PIN]);
  delay(1000);
  controlSolenoid->updateControl(6.75);
  assertEqual(HIGH, state->digitalPin[PIN]);
}

unittest(beforeTenSecondsButPhIsLower) {
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  assertEqual(HIGH, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(8.00);
  assertEqual(LOW, state->digitalPin[PIN]);
  delay(7500);
  controlSolenoid->updateControl(6.75);
  assertEqual(HIGH, state->digitalPin[PIN]);
}

unittest(PhEvenWithTarget) {
  GodmodeState* state = GODMODE();
  PHControl* controlSolenoid = PHControl::instance();
  assertEqual(HIGH, state->digitalPin[PIN]);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->updateControl(7.00);
  assertEqual(HIGH, state->digitalPin[PIN]);
}

unittest_main()
