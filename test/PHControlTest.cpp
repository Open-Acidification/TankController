#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "Devices/DataLogger_TC.h"
#include "Devices/DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "MainMenu.h"
#include "PHCalibrationMid.h"
#include "PHControl.h"
#include "TankController.h"

const uint16_t PH_CONTROL_PIN = 49;

GodmodeState* state = GODMODE();
TankController* tc = TankController::instance();
PHControl* controlSolenoid = PHControl::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
DataLogger_TC* dataLog = DataLogger_TC::instance();

void setPhMeasurementTo(float value) {
  char buffer[10];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i.%03i\r"), (int)value, (int)(value * 1000 + 0.5) % 1000);
  state->serialPort[1].dataIn = buffer;  // the queue of data waiting to be read
  tc->serialEvent1();                    // fake interrupt to update the current pH reading
}

/**
 * cycle the control through to a point of being off
 */
void reset() {
  tc->setNextState(new MainMenu(tc), true);
  state->resetClock();
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
  controlSolenoid->enablePID(false);
  setPhMeasurementTo(7.50);
  controlSolenoid->setBaseTargetPh(7.50);
  controlSolenoid->setRampDuration(0);  // No ramp
  state->serialPort[0].dataOut = "";    // the history of data written
  tc->loop(false);
}

unittest_setup() {
  reset();
}

unittest_teardown() {
  reset();
}

// During the first time window the pH is high, so we turn on the solenoid
unittest(bubblerTurnsOnAndOff) {
  assertEqual(6, millis());
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  state->serialPort[1].dataIn = "8.00\r";  // the queue of data waiting to be read
  assertEqual(6, millis());
  tc->serialEvent1();  // fake interrupt to update the current pH reading
  assertEqual(6, millis());
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(10, millis());
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  String serialOutput = state->serialPort[0].dataOut;
  String line;
  int i = 0, j = 0;
  do {
    j = serialOutput.indexOf('\r', i);
    if (j == -1) {
      assertTrue(false);  // we came to the end without finding the expected line
      break;
    }
    assertEqual('\r', serialOutput.charAt(j));
    line = serialOutput.substr(i, j - i);
    i = j + 2;
  } while (line.charAt(0) != 'C');
  assertEqual((int)'C', line.charAt(0));
  assertEqual("CO2 bubbler turned on after 6 ms", line);
  assertEqual(10, millis());
  delay(9500);
  tc->loop(false);  // solenoid should turn off briefly at end of window
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest(afterTenSecondsButPhStillHigher) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setBaseTargetPh(7.50);
  setPhMeasurementTo(8.50);
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(8000);
  setPhMeasurementTo(8.50);
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(2000);
  setPhMeasurementTo(7.75);
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
}

unittest(afterTenSecondsAndPhIsLower) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  assertEqual("pH 7.500   7.500", lc->getLines().at(0));
  controlSolenoid->setBaseTargetPh(7.50);
  setPhMeasurementTo(8.50);
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  assertEqual("CO2 bubbler turned on after 6 ms\r\n", state->serialPort[0].dataOut);
  tc->loop(false);
  assertEqual("pH 8.500 B 7.500", lc->getLines().at(0));
  delay(8000);
  tc->loop(false);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(1000);
  tc->loop(false);
  assertEqual("CO2 bubbler turned off after 9004 ms\r\n", state->serialPort[0].dataOut);  // after 10 seconds
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  delay(1000);
  setPhMeasurementTo(7.25);
  tc->loop(false);  // update the controls based on the current readings
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
  setPhMeasurementTo(8.50);
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(7500);
  setPhMeasurementTo(7.25);
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest(PhEvenWithTarget) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setBaseTargetPh(7.50);
  setPhMeasurementTo(7.50);
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

/**
 * Test that CO2 bubbler is turned on when needed
 * \see unittest(beforeTenSecondsButPhIsLower)
 */
unittest(disableDuringCalibration) {
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid(tc, 3);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // device is initially off and stays off due to calibration
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setBaseTargetPh(7.50);
  setPhMeasurementTo(8.50);
  tc->loop(false);  // update the controls based on the current readings
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());

  // device remains off between calibration states
  test->setValue(7.00);
  tc->loop(false);
  // assertEqual("Wait", tc->stateName());
  // delay(2000);
  // tc->loop(false);
  // assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  // assertFalse(controlSolenoid->isOn());
}

unittest(RampGreaterThanZero) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  delay(11000);
  setPhMeasurementTo(8.50);
  controlSolenoid->setBaseTargetPh(7.00);
  controlSolenoid->setRampDuration(1.5);  // 90 minutes
  assertEqual(PHControl::RAMP_TYPE, controlSolenoid->getPhSetType());
  tc->loop(false);
  assertEqual(8.5, controlSolenoid->getCurrentTargetPh());
  assertEqual("pH=8.500   8.500", lc->getLines().at(0));
  assertEqual("01/15/2021 01:48:35,   0, 0.00, 20.00, 8.500, 8.500,   11, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  // mock arduino restarting
  PHControl::clearInstance();
  controlSolenoid = PHControl::instance();
  // takes 1.5 hours to get to pH of 7
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertTrue(8.0 <= controlSolenoid->getCurrentTargetPh() && controlSolenoid->getCurrentTargetPh() <= 8.01);
  assertEqual("pH=8.500 B 8.000", lc->getLines().at(0));
  assertEqual("01/15/2021 02:18:35,   0, 0.00, 20.00, 8.500, 8.000, 1811, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  delay(1800000);  // delay 30 minutes
  // First loop triggers SD logging (DataLogger_TC) and PushingBox
  // Second loop triggers Serial logging (DataLogger_TC)
  tc->loop(false);
  assertTrue(7.5 <= controlSolenoid->getCurrentTargetPh() && controlSolenoid->getCurrentTargetPh() <= 7.51);
  assertEqual("pH=8.500 B 7.500", lc->getLines().at(0));
  assertEqual("01/15/2021 02:48:35,   0, 0.00, 20.00, 8.500, 7.500, 3611, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  assertEqual("pH=8.500 B 7.000", lc->getLines().at(0));
  assertEqual("01/15/2021 03:18:35,   0, 0.00, 20.00, 8.500, 7.000, 5411, 100000.0,      0.0,      0.0",
              dataLog->buffer);
  // ramp time no longer used after it ends
  delay(1800000);  // delay 30 minutes
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  assertEqual("pH=8.500 B 7.000", lc->getLines().at(0));
  assertEqual("01/15/2021 04:18:35,   0, 0.00, 20.00, 8.500, 7.000, 9011, 100000.0,      0.0,      0.0",
              dataLog->buffer);
}

unittest(ChangeRampToZero) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  setPhMeasurementTo(8.50);
  tc->loop(false);  // update the controls based on the current readings
  controlSolenoid->setBaseTargetPh(7.00);
  controlSolenoid->setRampDuration(1.5);
  assertEqual(PHControl::RAMP_TYPE, controlSolenoid->getPhSetType());
  tc->loop(false);
  assertEqual(8.5, controlSolenoid->getCurrentTargetPh());
  controlSolenoid->setRampDuration(0);
  assertEqual(PHControl::FLAT_TYPE, controlSolenoid->getPhSetType());
  tc->loop(false);
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
}

unittest(sineTest) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  setPhMeasurementTo(7.00);
  tc->loop(false);  // update the controls based on the current readings
  controlSolenoid->setBaseTargetPh(7.00);
  controlSolenoid->setSine(1.5, 2);
  assertEqual(PHControl::SINE_TYPE, controlSolenoid->getPhSetType());
  tc->loop(false);
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  // mock arduino restarting
  PHControl::clearInstance();
  controlSolenoid = PHControl::instance();
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(8.5, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(5.5, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  // make sure sine wave continues
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(8.5, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(5.5, controlSolenoid->getCurrentTargetPh());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(7, controlSolenoid->getCurrentTargetPh());
}

unittest_main()
