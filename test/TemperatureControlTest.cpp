#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>


#include "TankControllerLib.h"
#include "TemperatureControl.h"

/*
bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=TemperatureControlTest.cpp
*/

//  void Chiller::updateControl(double currentTemperature);   // void TemperatureControl::turnOn(bool newState) {
unittest(chillerUpdateControlWithinDelta) {
    Chiller chiller;
    chiller.setTargetTemperature(30.04);
    delay(31000);
    chiller.updateControl(30);
    assertEqual(true, chiller.getCurrentSwitchState);
}

unittest(chillerUpdateControlbBeforeInterval) {
    Chiller chiller;
    chiller.setTargetTemperature(30.05);
    chiller.updateControl(30);
    assertEqual(true, chiller.getCurrentSwitchState);
}

unittest(chillerUpdateControlbAfterInterval) {
    Chiller chiller;
    chiller.setTargetTemperature(30.05);
    delay(31000);
    chiller.updateControl(30);
    assertEqual(false, chiller.getCurrentSwitchState);
}
