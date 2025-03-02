#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "DataLogger.h"
#include "DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "MainMenu.h"
#include "PHCalibrationMid.h"
#include "PHControl.h"
#include "PHProbe.h"
#include "TankController.h"

const uint16_t PH_CONTROL_PIN = 49;

GodmodeState* state = GODMODE();
TankController* tc = TankController::instance();
PHControl* controlSolenoid = PHControl::instance();
PHProbe* pHProbe = PHProbe::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
DataLogger* dataLog = DataLogger::instance();

/**
 * cycle the control through to a point of being off
 */
void reset() {
  tc->setNextState(new MainMenu(), true);
  state->resetClock();
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
  controlSolenoid->enablePID(false);
  pHProbe->setPh(7.5);
  controlSolenoid->setBaseTargetPh(7.50);
  controlSolenoid->setRampDurationHours(0);  // No ramp
  tc->loop();
}

unittest_setup() {
  reset();
}

unittest_teardown() {
  reset();
}

// During the first time window the pH is high, so we turn on the solenoid
unittest(bubblerTurnsOnAndOff) {
  assertEqual(10, millis());
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  pHProbe->setPh(8.0);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(9500);
  tc->loop();  // solenoid should turn off briefly at end of window
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest(afterTenSecondsButPhStillHigher) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setBaseTargetPh(7.50);
  pHProbe->setPh(8.5);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(8000);
  pHProbe->setPh(8.5);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(2000);
  pHProbe->setPh(7.75);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
}

unittest(afterTenSecondsAndPhIsLower) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  assertEqual("pH 7.500   7.500", lc->getLines().at(0));
  controlSolenoid->setBaseTargetPh(7.50);
  pHProbe->setPh(8.5);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  tc->loop();
  assertEqual("pH 8.500 B 7.500", lc->getLines().at(0));
  delay(8000);
  tc->loop();
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(1000);
  tc->loop();
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  delay(1000);
  pHProbe->setPh(7.25);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

/**
 * Test that CO2 bubbler is turned on when needed
 * \see unittest(disableDuringCalibration)
 */
unittest(beforeTenSecondsButPhIsLower) {
  // device is initially off but turns on when needed
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  delay(1000);
  controlSolenoid->setBaseTargetPh(7.50);
  pHProbe->setPh(8.5);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(7500);
  pHProbe->setPh(7.25);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest(PhEvenWithTarget) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setBaseTargetPh(7.50);
  pHProbe->setPh(7.5);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

/**
 * Test that CO2 bubbler is turned on when needed
 * \see unittest(beforeTenSecondsButPhIsLower)
 */
unittest(disableDuringCalibration) {
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // device is initially off and stays off due to calibration
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setBaseTargetPh(7.50);
  pHProbe->setPh(8.5);
  tc->loop();  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  tc->loop();

  // device remains off between calibration states
  test->setValue(7.00);
  tc->loop();
  assertEqual("Wait", tc->stateName());
  delay(2000);
  tc->loop();
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  tc->loop();
}

unittest(RampGreaterThanZero) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  delay(11000);
  pHProbe->setPh(8.5);
  controlSolenoid->setBaseTargetPh(7.00);
  controlSolenoid->setRampDurationHours(1.5);  // 90 minutes
  assertEqual(PHControl::RAMP_TYPE, controlSolenoid->getPHFunctionType());
  tc->loop();
  assertEqual(8.5, controlSolenoid->getCurrentTargetPh());
  assertEqual("pH=8.500   8.500", lc->getLines().at(0));
  // mock arduino restarting
  PHControl::clearInstance();
  controlSolenoid = PHControl::instance();
  // takes 1.5 hours to get to pH of 7
  delay(1800000);  // delay 30 minutes
  tc->loop();
  tc->loop();
  assertTrue(8.0 <= controlSolenoid->getCurrentTargetPh() && controlSolenoid->getCurrentTargetPh() <= 8.01);
  assertEqual("pH=8.500 B 8.000", lc->getLines().at(0));
  assertEqual("01/15/2021 02:18:35,   0, 0.00, 20.00, 8.500, 8.000, 1811, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  delay(1800000);  // delay 30 minutes
  // First loop triggers SD logging (DataLogger) and PushingBox
  // Second loop triggers Serial logging (DataLogger)
  tc->loop();
  tc->loop();
  assertTrue(7.5 <= controlSolenoid->getCurrentTargetPh() && controlSolenoid->getCurrentTargetPh() <= 7.51);
  assertEqual("pH=8.500 B 7.500", lc->getLines().at(0));
  assertEqual("01/15/2021 02:48:35,   0, 0.00, 20.00, 8.500, 7.500, 3611, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  delay(1800000);  // delay 30 minutes
  tc->loop();
  tc->loop();
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  assertEqual("pH=8.500 B 7.000", lc->getLines().at(0));
  assertEqual("01/15/2021 03:18:35,   0, 0.00, 20.00, 8.500, 7.000, 5411, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  // ramp time no longer used after it ends
  delay(1800000);  // delay 30 minutes
  delay(1800000);  // delay 30 minutes
  tc->loop();
  tc->loop();
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  assertEqual("pH=8.500 B 7.000", lc->getLines().at(0));
  assertEqual("01/15/2021 04:18:35,   0, 0.00, 20.00, 8.500, 7.000, 9011, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
}

unittest(ChangeRampToZero) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  pHProbe->setPh(8.5);
  tc->loop();  // update the controls based on the current readings
  controlSolenoid->setBaseTargetPh(7.00);
  controlSolenoid->setRampDurationHours(1.5);
  assertEqual(PHControl::RAMP_TYPE, controlSolenoid->getPHFunctionType());
  tc->loop();
  assertEqual(8.5, controlSolenoid->getCurrentTargetPh());
  controlSolenoid->setRampDurationHours(0);
  assertEqual(PHControl::FLAT_TYPE, controlSolenoid->getPHFunctionType());
  tc->loop();
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
}

unittest(sineTest) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  pHProbe->setPh(7.0);
  tc->loop();  // update the controls based on the current readings
  controlSolenoid->setBaseTargetPh(7.00);
  controlSolenoid->setSineAmplitudeAndHours(1.5, 2);
  assertEqual(PHControl::SINE_TYPE, controlSolenoid->getPHFunctionType());
  tc->loop();
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  // mock arduino restarting
  PHControl::clearInstance();
  controlSolenoid = PHControl::instance();
  delay(1800000);  // delay 30 minutes
  tc->loop();
  assertEqual(8.5, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop();
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop();
  assertEqual(5.5, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop();
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  // make sure sine wave continues
  delay(1800000);  // delay 30 minutes
  tc->loop();
  assertEqual(8.5, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop();
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop();
  assertEqual(5.5, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop();
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
}

unittest_main()
