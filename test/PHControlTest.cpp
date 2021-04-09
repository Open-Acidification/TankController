#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "PHControl.h"

//updateControl function
unittest(beforeTenSeconds) {
  GodmodeState* state = GODMODE();
  state->reset();
  PHControl controlSolenoid;
  assertEqual(HIGH, state->digitalPin[controlSolenoid.getPIN()]);
  controlSolenoid.setTargetPh(7.00);
  controlSolenoid.setUsePID(false);
  controlSolenoid.updateControl(8.00);
  assertEqual(LOW, state->digitalPin[controlSolenoid.getPIN()]);
  delay(9500);
  controlSolenoid.updateControl(8.00);
  assertEqual(LOW, state->digitalPin[controlSolenoid.getPIN()]);
}

unittest(afterTenSecondsButPhStillHigher) {
  GodmodeState* state = GODMODE();
  state->reset();
  PHControl controlSolenoid;
  assertEqual(HIGH, state->digitalPin[controlSolenoid.getPIN()]);
  controlSolenoid.setTargetPh(7.00);
  controlSolenoid.setUsePID(false);
  controlSolenoid.updateControl(8.00);
  assertEqual(LOW, state->digitalPin[controlSolenoid.getPIN()]);
  delay(9500);
  controlSolenoid.updateControl(8.00);
  assertEqual(LOW, state->digitalPin[controlSolenoid.getPIN()]);
  delay(1000);
  controlSolenoid.updateControl(7.25);
  assertEqual(LOW, state->digitalPin[controlSolenoid.getPIN()]);
}

unittest(afterTenSecondsAndPhIsLower) {
  GodmodeState* state = GODMODE();
  state->reset();
  PHControl controlSolenoid;
  assertEqual(HIGH, state->digitalPin[controlSolenoid.getPIN()]);
  controlSolenoid.setTargetPh(7.00);
  controlSolenoid.setUsePID(false);
  controlSolenoid.updateControl(8.00);
  assertEqual(LOW, state->digitalPin[controlSolenoid.getPIN()]);
  delay(9500);
  controlSolenoid.updateControl(8.00);
  assertEqual(LOW, state->digitalPin[controlSolenoid.getPIN()]);
  delay(1000);
  controlSolenoid.updateControl(6.75);
  assertEqual(HIGH, state->digitalPin[controlSolenoid.getPIN()]);
}

unittest(beforeTenSecondsButPhIsLower) {
  GodmodeState* state = GODMODE();
  state->reset();
  PHControl controlSolenoid;
  assertEqual(HIGH, state->digitalPin[controlSolenoid.getPIN()]);
  controlSolenoid.setTargetPh(7.00);
  controlSolenoid.setUsePID(false);
  controlSolenoid.updateControl(8.00);
  assertEqual(LOW, state->digitalPin[controlSolenoid.getPIN()]);
  delay(7500);
  controlSolenoid.updateControl(6.75);
  assertEqual(HIGH, state->digitalPin[controlSolenoid.getPIN()]);
}

unittest(PhEvenWithTarget) {
  GodmodeState* state = GODMODE();
  state->reset();
  PHControl controlSolenoid;
  assertEqual(HIGH, state->digitalPin[controlSolenoid.getPIN()]);
  controlSolenoid.setTargetPh(7.00);
  controlSolenoid.setUsePID(false);
  controlSolenoid.updateControl(7.00);
  assertEqual(HIGH, state->digitalPin[controlSolenoid.getPIN()]);
}

unittest_main()
