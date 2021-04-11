#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "TemperatureControl.h"
/**
 * These tests test the UpdateControl virtual function for the heater and chiller subclass and
 * checks to see whether the heater or chiller should be in an on state or an off state.
 * The following command lets you skip all the other tests and only run these tests
 * bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=TemperatureControlTest.cpp
 */

// Chiller
unittest(BeforeIntervalAndWithinDelta) {
  const int PIN = 47;
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  assertEqual(HIGH, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  chiller.updateControl(20.04);
  assertEqual(HIGH, state->digitalPin[PIN]);
}

// Chiller
unittest(BeforeIntervalAndOutsideDelta) {
  const int PIN = 47;
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  assertEqual(HIGH, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  chiller.updateControl(20.05);
  assertEqual(HIGH, state->digitalPin[PIN]);
}

// Chiller
unittest(AfterIntervalAndWithinDelta) {
  const int PIN = 47;
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  assertEqual(HIGH, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  delay(31000);
  chiller.updateControl(20.04);
  assertEqual(HIGH, state->digitalPin[PIN]);
}

// Chiller
unittest(AfterIntervalAndOutsideDelta) {
  const int PIN = 47;
  GodmodeState* state = GODMODE();
  state->reset();
  Chiller chiller;
  assertEqual(HIGH, state->digitalPin[PIN]);
  chiller.setTargetTemperature(20);
  delay(31000);
  chiller.updateControl(20.05);
  assertEqual(LOW, state->digitalPin[PIN]);
}

// Heater
unittest(WithinDelta) {
  const int PIN = 47;
  GodmodeState* state = GODMODE();
  state->reset();
  Heater heater;
  assertEqual(HIGH, state->digitalPin[PIN]);
  heater.setTargetTemperature(20);
  heater.updateControl(19.96);
  assertEqual(HIGH, state->digitalPin[PIN]);
}

// Heater
unittest(OutsideDelta) {
  const int PIN = 47;
  GodmodeState* state = GODMODE();
  state->reset();
  Heater heater;
  assertEqual(HIGH, state->digitalPin[PIN]);
  heater.setTargetTemperature(20);
  heater.updateControl(19.95);
  assertEqual(LOW, state->digitalPin[PIN]);
}

unittest_main()
