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

void setPhMeasurementTo(float value) {
  char buffer[10];
  snprintf(buffer, sizeof(buffer), "%f\r", value);
  state->serialPort[1].dataIn = buffer;  // the queue of data waiting to be read
  tc->serialEvent1();                    // fake interrupt to update the current pH reading
  tc->loop();                            // update the controls based on the current readings
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
  controlSolenoid->setTargetPh(7.50);
  setPhMeasurementTo(7.50);
  state->serialPort[0].dataOut = "";  // the history of data written
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
  tc->serialEvent1();                      // fake interrupt to update the current pH reading
  tc->loop();                              // update the controls based on the current readings
  assertEqual(13, millis());
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
    line = serialOutput.substr(i, j);
    i = j + 2;
  } while (line.charAt(0) != 'C');
  assertEqual((int)'C', line.charAt(0));
  assertEqual("CO2 bubbler turned on after 13 ms", line);
  assertEqual(13, millis());
  delay(9500);
  tc->loop();  // solenoid should turn off briefly at end of window
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest(afterTenSecondsButPhStillHigher) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setTargetPh(7.50);
  setPhMeasurementTo(8.50);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(8000);
  setPhMeasurementTo(8.50);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(2000);
  setPhMeasurementTo(7.75);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
}

unittest(afterTenSecondsAndPhIsLower) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  assertEqual("pH 7.500   7.500", lc->getLines().at(0));
  controlSolenoid->setTargetPh(7.50);
  setPhMeasurementTo(8.50);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  assertEqual("CO2 bubbler turned on after 7 ms\r\n", state->serialPort[0].dataOut);
  tc->loop();
  assertEqual("pH 8.500 B 7.500", lc->getLines().at(0));
  delay(8000);
  tc->loop();
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(1000);
  tc->loop();
  assertEqual("CO2 bubbler turned off after 9021 ms\r\n", state->serialPort[0].dataOut);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  delay(1000);
  setPhMeasurementTo(7.25);
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
  controlSolenoid->setTargetPh(7.50);
  setPhMeasurementTo(8.50);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_CONTROL_PIN]);
  assertTrue(controlSolenoid->isOn());
  delay(7500);
  setPhMeasurementTo(7.25);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest(PhEvenWithTarget) {
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
  controlSolenoid->setTargetPh(7.50);
  setPhMeasurementTo(7.50);
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
  controlSolenoid->setTargetPh(7.50);
  setPhMeasurementTo(8.50);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_CONTROL_PIN]);
  assertFalse(controlSolenoid->isOn());
}

unittest_main()
