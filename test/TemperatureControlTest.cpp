#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "TankControllerLib.h"
#include "TemperatureControl.h"
/**
 * These tests test the UpdateControl virtual function for the heater and chiller subclass and
 * checks to see whether the heater or chiller should be in an on state or an off state.
 * The following command lets you skip all the other tests and only run these tests
 * bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=TemperatureControlTest.cpp
 */

// Chiller
unittest(BeforeIntervalAndWithinDelta) {
  Chiller chiller;
  assertFalse(chiller.getCurrentSwitchState());
  chiller.setTargetTemperature(20);
  chiller.updateControl(20.04);
  assertFalse(chiller.getCurrentSwitchState());
}

// Chiller
unittest(BeforeIntervalAndOutsideDelta) {
  Chiller chiller;
  assertFalse(chiller.getCurrentSwitchState());
  chiller.setTargetTemperature(20);
  chiller.updateControl(20.05);
  assertFalse(chiller.getCurrentSwitchState());
}

// Chiller
unittest(AfterIntervalAndWithinDelta) {
  Chiller chiller;
  assertFalse(chiller.getCurrentSwitchState());
  chiller.setTargetTemperature(20);
  delay(31000);
  chiller.updateControl(20.04);
  assertFalse(chiller.getCurrentSwitchState());
}

// Chiller
unittest(AfterIntervalAndOutsideDelta) {
  Chiller chiller;
  assertFalse(chiller.getCurrentSwitchState());
  chiller.setTargetTemperature(20);
  delay(31000);
  chiller.updateControl(20.05);
  assertTrue(chiller.getCurrentSwitchState());
}

// Heater
unittest(WithinDelta) {
  Heater heater;
  assertFalse(heater.getCurrentSwitchState());
  heater.setTargetTemperature(20);
  heater.updateControl(19.96);
  assertFalse(heater.getCurrentSwitchState());
}

// Heater
unittest(OutsideDelta) {
  Heater heater;
  assertFalse(heater.getCurrentSwitchState());
  heater.setTargetTemperature(20);
  heater.updateControl(19.95);
  assertTrue(heater.getCurrentSwitchState());
}

unittest_main()
