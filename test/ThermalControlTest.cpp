#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include "DataLogger.h"
#include "DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "MainMenu.h"
#include "PHCalibrationMid.h"
#include "Serial_TC.h"
#include "TankController.h"
#include "ThermalControl.h"
#include "ThermalProbe_TC.h"
/**
 * These tests test the UpdateControl virtual function for the heater and chiller subclass and
 * checks to see whether the heater or chiller should be in an on state or an off state.
 * The following command lets you skip all the other tests and only run these tests
 * bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=ThermalControlTest.cpp
 */

const uint16_t THERMAL_CONTROL_PIN = 47;
DataLogger* dataLog = DataLogger::instance();
GodmodeState* state = GODMODE();
TankController* tc = TankController::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
ThermalControl* control;

unittest_setup() {
  state->resetClock();
  thermalProbe->setTemperature(20, true);
  tc->setNextState(new MainMenu(), true);
  state->serialPort[0].dataOut = "";  // the history of data written
  DataLogger::instance()->clearBuffer();
}

unittest_teardown() {
  tc->setNextState(new MainMenu(), true);
}

// Chiller
unittest(BeforeIntervalAndWithinDelta) {
  ThermalControl::enableHeater(false);
  control = ThermalControl::instance();
  control->setThermalTarget(20);
  control->updateControl(20);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  control->updateControl(20.04);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
}

// Chiller
unittest(BeforeIntervalAndOutsideDelta) {
  ThermalControl::enableHeater(false);
  control = ThermalControl::instance();
  control->setThermalTarget(20);
  control->updateControl(20);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  control->updateControl(20.05);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
}

// Chiller
unittest(AfterIntervalAndWithinDelta) {
  ThermalControl::enableHeater(false);
  control = ThermalControl::instance();
  control->setThermalTarget(20);
  control->updateControl(20);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  delay(31000);
  control->updateControl(20.04);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
}

/**
 * Test that is turned on when needed
 * \see unittest(disableChillerDuringCalibration)
 */
unittest(AfterIntervalAndOutsideDelta) {
  ThermalControl::enableHeater(false);
  control = ThermalControl::instance();
  control->setThermalTarget(20);
  control->updateControl(20);
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
  state->serialPort[0].dataOut = "";  // the history of data written
  // chiller is initially off and goes on when needed
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  assertEqual(0, millis());
  delay(31006);
  assertEqual(31006, millis());
  control->updateControl(20.05);
  assertTrue(control->isOn());
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[THERMAL_CONTROL_PIN]);
  assertEqual("chiller turned on at 31006 after 31006 ms", Serial_TC::instance()->getBuffer());
  tc->loop(false);
  assertEqual("T=20.02 C 20.00 ", lc->getLines().at(1));
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(31012);
  control->updateControl(19.95);
  assertFalse(control->isOn());
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  assertEqual("chiller turned off at 62024 after 31018 ms", Serial_TC::instance()->getBuffer());
  tc->loop(false);
  tc->loop(false);
  assertEqual("T 20.02 c 20.00 ", lc->getLines().at(1));
  assertEqual("01/15/2021 01:49:26,   0, 20.02, 20.00, 0.000, 8.100,   62, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
}

/**
 * Test that chiller is not turned on during calibration
 * \see unittest(AfterIntervalAndOutsideDelta)
 */
unittest(disableChillerDuringCalibration) {
  ThermalControl::enableHeater(false);
  control = ThermalControl::instance();
  control->setThermalTarget(20);
  control->updateControl(20);
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // chiller is initially off and stays off during calibration
  // (test is same as above)
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  delay(31000);
  control->updateControl(20.05);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
}

// Heater
unittest(WithinDelta) {
  ThermalControl::enableHeater(true);
  control = ThermalControl::instance();
  control->setThermalTarget(20);
  control->updateControl(20);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  control->updateControl(19.96);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
}

/**
 * Test that heater is turned on when needed
 * \see unittest(disableHeaterDuringCalibration)
 */
unittest(OutsideDelta) {
  ThermalControl::enableHeater(true);
  control = ThermalControl::instance();
  control->setThermalTarget(20);
  control->updateControl(20);
  // heater is initially off, then turns on
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  control->updateControl(19.95);
  assertTrue(control->isOn());
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[THERMAL_CONTROL_PIN]);
  assertEqual("heater turned on at 0 after 0 ms", Serial_TC::instance()->getBuffer());
  tc->loop(false);
  assertEqual("T 20.00 H 20.00 ", lc->getLines().at(1));
  state->serialPort[0].dataOut = "";  // the history of data written
  delay(300);
  control->updateControl(20.05);
  assertFalse(control->isOn());
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  assertEqual("heater turned off at 306 after 306 ms", Serial_TC::instance()->getBuffer());
  tc->loop(false);
  assertEqual("T 20.00 h 20.00 ", lc->getLines().at(1));
}

/**
 * Test that heater is not turned on during calibration
 * \see unittest(OutsideDelta)
 */
unittest(disableHeaterDuringCalibration) {
  ThermalControl::enableHeater(true);
  control = ThermalControl::instance();
  assertFalse(tc->isInCalibration());
  PHCalibrationMid* test = new PHCalibrationMid();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // heater is initially off, and stays off due to calibration
  // (test is same as above)
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
  control->setThermalTarget(20);
  control->updateControl(19.95);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[THERMAL_CONTROL_PIN]);
}

unittest(RampGreaterThanZero) {
  float target = 0.0;
  ThermalControl::enableHeater(false);
  control = ThermalControl::instance();
  assertFalse(control->isOn());
  control->setThermalTarget(10);
  control->setRampDurationHours(1.5);
  assertEqual(ThermalControl::thermalFunctionTypes::RAMP_TYPE, control->getThermalFunctionType());
  assertEqual("", dataLog->getBuffer());  // data left over from previous tests is cleared
  tc->loop(false);
  char* pBuffer = dataLog->getBuffer();
  int i = 0;
  int tabCount = 0;
  while (pBuffer[i] != '\0') {
    if (pBuffer[i] == '\t') {
      tabCount++;
      if (tabCount == 29) {
        break;
      }
    }
    i++;
  }
  assertEqual(29, tabCount);  // beginning of thermal target ("10.00")
  ++i;
  for (int j = 0; j < 5; j++) {
    assertTrue(pBuffer[i + j] != '\0');
  }
  assertEqual('\t', pBuffer[i + 5]);  // end of thermal target
  ++tabCount;
  pBuffer[i + 5] = '\0';
  assertEqual("10.00", &pBuffer[i]);
  pBuffer[i + 5] = '\t';
  i += 6;
  int j = i;  // now at beginning of ramp start time
  while (pBuffer[j] != '\0') {
    if (pBuffer[j] == '\t') {
      tabCount++;
      break;
    }
    j++;
  }
  assertEqual(31, tabCount);  // found end of ramp start time
  pBuffer[j] = '\0';
  uint64_t rampStart = strtoull(&pBuffer[i], NULL, 10);
  pBuffer[j] = '\t';
  i = j + 1;
  j = i;  // now at beginning of ramp end time
  while (pBuffer[j] != '\0') {
    if (pBuffer[j] == '\t') {
      tabCount++;
      break;
    }
    j++;
  }
  assertEqual(32, tabCount);  // found end of ramp end time
  pBuffer[j] = '\0';
  uint64_t rampEnd = strtoull(&pBuffer[i], NULL, 10);
  pBuffer[j] = '\t';
  uint64_t rampDuration = rampEnd - rampStart;
  assertEqual(90 * 60, rampDuration);
  dataLog->clearBuffer();
  assertEqual("", dataLog->getBuffer());  // data left over from previous tests is cleared
  control->updateControl(thermalProbe->getRunningAverage());
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentThermalTarget();
  assertTrue(abs(20 - target) < 0.1);
  assertEqual("T 20.00 c 20.00 ", lc->getLines().at(1));
  assertEqual("", dataLog->getBuffer());  // data left over from previous tests is cleared
  delay(31000);
  // mock arduino restarting
  ThermalControl::clearInstance();
  control = ThermalControl::instance();
  // takes 1.5 hours to get to Temp of 7
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentThermalTarget();
  assertTrue(16.6 <= target && target <= 16.8);
  assertEqual("T=20.00 C 16.61 ", lc->getLines().at(1));
  assertEqual("01/15/2021 02:18:55,   0, 20.00, 16.61, 0.000, 8.100, 1831, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentThermalTarget();
  assertTrue(13.2 <= target && target <= 13.4);
  assertEqual("T=20.00 C 13.28 ", lc->getLines().at(1));
  assertEqual("01/15/2021 02:48:55,   0, 20.00, 13.28, 0.000, 8.100, 3631, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  assertEqual(10, control->getCurrentThermalTarget());
  assertEqual("T=20.01 C 10.00 ", lc->getLines().at(1));
  assertEqual("01/15/2021 03:18:55,   0, 20.01, 10.00, 0.000, 8.100, 5431, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  // ramp time no longer used after it ends
  delay(1800000);  // delay 30 minutes
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  assertEqual(10, control->getCurrentThermalTarget());
  assertEqual("T=20.01 C 10.00 ", lc->getLines().at(1));
  assertEqual("01/15/2021 04:18:55,   0, 20.01, 10.00, 0.000, 8.100, 9031, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  delay(31000);
  ThermalControl::enableHeater(true);
  control = ThermalControl::instance();
  assertFalse(control->isOn());
  control->setThermalTarget(30);
  control->setRampDurationHours(1.5);
  control->updateControl(thermalProbe->getRunningAverage());
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentThermalTarget();
  assertTrue(20 <= target && target <= 20.03);
  assertEqual("T 20.01 h 20.01 ", lc->getLines().at(1));
  assertEqual("01/15/2021 04:19:26,   0, 20.01, 20.01, 0.000, 8.100, 9062, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  // mock arduino restarting
  ThermalControl::clearInstance();
  control = ThermalControl::instance();
  // takes 1.5 hours to get to Temp of 7
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentThermalTarget();
  assertTrue(23.3 <= target && target <= 23.4);
  assertEqual("T 20.01 H 23.34 ", lc->getLines().at(1));
  assertEqual("01/15/2021 04:49:26,   0, 20.01, 23.34, 0.000, 8.100, 10862, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  target = control->getCurrentThermalTarget();
  assertTrue(26.6 <= target && target <= 26.7);
  assertEqual("T 20.01 H 26.67 ", lc->getLines().at(1));
  assertEqual("01/15/2021 05:19:26,   0, 20.01, 26.67, 0.000, 8.100, 12662, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
  // ramp time no longer used after it ends
  delay(1800000);  // delay 30 minutes
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
  assertEqual("T 20.02 H 30.00 ", lc->getLines().at(1));
  assertEqual("01/15/2021 06:49:26,   0, 20.02, 30.00, 0.000, 8.100, 18062, 100000.0,      0.0,      0.0",
              dataLog->getBuffer());
}

unittest(ChangeRampToZero) {
  ThermalControl::enableHeater(false);
  control = ThermalControl::instance();
  assertFalse(control->isOn());
  control->setThermalTarget(10);
  control->setRampDurationHours(1.5);
  tc->loop(false);
  assertTrue(20 <= control->getCurrentThermalTarget() && control->getCurrentThermalTarget() <= 20.03);
  control->setRampDurationHours(0);
  tc->loop(false);
  assertEqual(10, control->getCurrentThermalTarget());
  ThermalControl::enableHeater(true);
  control = ThermalControl::instance();
  assertFalse(control->isOn());
  control->setThermalTarget(30);
  control->setRampDurationHours(1.5);
  tc->loop(false);
  assertTrue(20 <= control->getCurrentThermalTarget() && control->getCurrentThermalTarget() <= 20.03);
  control->setRampDurationHours(0);
  assertEqual(ThermalControl::thermalFunctionTypes::FLAT_TYPE, control->getThermalFunctionType());
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
}

unittest(sineTest) {
  ThermalControl::enableHeater(false);
  control = ThermalControl::instance();
  assertFalse(control->isOn());
  control->setThermalTarget(10);
  control->setSineAmplitudeAndHours(1.5, 2);
  assertEqual(ThermalControl::thermalFunctionTypes::SINE_TYPE, control->getThermalFunctionType());
  tc->loop(false);
  assertEqual(10, control->getCurrentThermalTarget());
  // mock arduino restarting
  ThermalControl::clearInstance();
  control = ThermalControl::instance();
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(11.5, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(10, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(8.5, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(10, control->getCurrentThermalTarget());
  // make sure sine wave continues
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(11.5, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(10, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(8.5, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(10, control->getCurrentThermalTarget());
  ThermalControl::enableHeater(true);
  control = ThermalControl::instance();
  assertFalse(control->isOn());
  control->setThermalTarget(30);
  control->setSineAmplitudeAndHours(1.5, 2);
  assertEqual(ThermalControl::thermalFunctionTypes::SINE_TYPE, control->getThermalFunctionType());
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
  // mock arduino restarting
  ThermalControl::clearInstance();
  control = ThermalControl::instance();
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(31.5, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(28.5, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
  // make sure sine wave continues
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(31.5, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(28.5, control->getCurrentThermalTarget());
  delay(1800000);  // delay 30 minutes
  tc->loop(false);
  assertEqual(30, control->getCurrentThermalTarget());
}

unittest_main()
