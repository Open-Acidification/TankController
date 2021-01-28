#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>


#include "TankControllerLib.h"
#include "TemperatureControl.h"

/*
bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=TemperatureControlTest.cpp
*/

//  void Chiller::updateControl(double currentTemperature);   // void TemperatureControl::turnOn(bool newState) {
unittest(chillerUpdateControlBeforeIntervalAndWithinDelta) {
    Chiller chiller;
    assertFalse(chiller.getCurrentSwitchState());
    chiller.setTargetTemperature(20);
    // delay(31000);
    chiller.updateControl(20.04);
    assertFalse(chiller.getCurrentSwitchState());
}

unittest(chillerUpdateControlbBeforeIntervalAndOutsideOfDelta) {
    Chiller chiller;
    assertFalse(chiller.getCurrentSwitchState());
    chiller.setTargetTemperature(20);
    // delay(31000);
    chiller.updateControl(20.05);
    assertFalse(chiller.getCurrentSwitchState());
}

unittest(chillerUpdateControlAfterIntervalAndWithinDelta) {
    Chiller chiller;
    assertFalse(chiller.getCurrentSwitchState());
    chiller.setTargetTemperature(20);
    delay(31000);
    chiller.updateControl(20.04);
    assertFalse(chiller.getCurrentSwitchState());
}

unittest(chillerUpdateControlAfterIntervalAndOutsideDelta) {
    Chiller chiller;
    assertFalse(chiller.getCurrentSwitchState());
    chiller.setTargetTemperature(20);
    delay(31000);
    chiller.updateControl(20.05);
    assertTrue(chiller.getCurrentSwitchState());
}

unittest(heaterUpdateControlWithinDelta) {
    Heater heater;
    assertFalse(heater.getCurrentSwitchState());
    heater.setTargetTemperature(20);
    heater.updateControl(19.96);
    assertFalse(heater.getCurrentSwitchState());
}

unittest(heaterUpdateControlOutsideDelta) {
    Heater heater;
    assertFalse(heater.getCurrentSwitchState());
    heater.setTargetTemperature(20);
    heater.updateControl(19.95);
    assertTrue(heater.getCurrentSwitchState());
}


unittest_main()
