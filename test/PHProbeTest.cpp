#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DataLogger.h"
#include "EEPROM_TC.h"
#include "PHProbe.h"
#include "Serial_TC.h"
#include "TC_util.h"
#include "TankController.h"

EEPROM_TC *eeprom = EEPROM_TC::instance();
PHProbe *pHProbe = PHProbe::instance();
TankController *tc = TankController::instance();

unittest(singleton) {
  PHProbe *singleton1 = PHProbe::instance();
  PHProbe *singleton2 = PHProbe::instance();
  assertEqual(singleton1, singleton2);
}

unittest(constructor) {
  assertEqual("*OK,0\rC,1\rSLOPE,?\r", GODMODE()->serialPort[1].dataOut);
}

// tests getPh() and getSlopeResponse as well
unittest(serialEvent1) {
  tc->loop();  // Writes something to EEPROM, triggering a DataLogger warning
  tc->loop();  // DataLogger writes to SD card
  DataLogger *dl = DataLogger::instance();
  dl->reset();
  GodmodeState *state = GODMODE();
  state->reset();
  state->serialPort[0].dataOut = "";
  assertEqual("", state->serialPort[1].dataOut);
  tc->serialEvent1();  // fake interrupt
  assertEqual("", pHProbe->getCalibrationResponse());
  assertEqual(0, pHProbe->getPh());
  assertEqual("Requesting...", pHProbe->getSlopeResponse());
  pHProbe->setCalibration(2);
  pHProbe->setPh(7.125);
  assertFalse(dl->getShouldWriteWarning());
  assertEqual("", dl->getBuffer());
  // Next line calls serialEvent1 (triggering warning) and tc->loop() (sending warning)
  pHProbe->setPhSlope();
  assertFalse(dl->getShouldWriteWarning());  // already false again
  string lastWrittenString(dl->getBuffer());
  assertTrue(lastWrittenString.find("99.7,100.3,-0.89") > 0);  // warning was sent
  assertEqual("PH Calibra: 2 pt", pHProbe->getCalibrationResponse());
  assertEqual(7.125, pHProbe->getPh());
  assertEqual("99.7,100.3,-0.89", pHProbe->getSlopeResponse());
}

unittest(serialEvent1CatchBadSlope) {
  eeprom->setIgnoreBadPHSlope(true);
  assertTrue(eeprom->getIgnoreBadPHSlope());
  tc->serialEvent1();
  assertFalse(pHProbe->slopeIsBad());
  assertFalse(pHProbe->shouldWarnAboutCalibration());

  // Bad slope
  pHProbe->setPhSlope("?SLOPE,-2.7,101.3\r");
  assertTrue(pHProbe->slopeIsBad());
  assertTrue(eeprom->getIgnoreBadPHSlope());
  assertFalse(pHProbe->shouldWarnAboutCalibration());
  assertEqual("BAD CALIBRATION? pH slopes are more than 5\% from ideal", Serial_TC::instance()->getBuffer());

  // Good slope
  pHProbe->setPhSlope();
  assertFalse(pHProbe->slopeIsBad());
  assertFalse(eeprom->getIgnoreBadPHSlope());
  assertFalse(pHProbe->shouldWarnAboutCalibration());
  assertEqual("pH slopes are within 5\% of ideal", Serial_TC::instance()->getBuffer());

  // Bad slope
  pHProbe->setPhSlope("?SLOPE,98.7,107.2,-0.89\r");
  assertTrue(pHProbe->slopeIsBad());
  assertEqual("BAD CALIBRATION? pH slopes are more than 5\% from ideal", Serial_TC::instance()->getBuffer());
}

unittest(clearCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  eeprom->setIgnoreBadPHSlope(true);
  assertTrue(eeprom->getIgnoreBadPHSlope());
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->clearCalibration();
  assertEqual("Cal,clear\r", state->serialPort[1].dataOut);
  assertFalse(eeprom->getIgnoreBadPHSlope());
  assertFalse(pHProbe->slopeIsBad());
}

unittest(clearBadCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  eeprom->setIgnoreBadPHSlope(true);
  assertTrue(eeprom->getIgnoreBadPHSlope());
  pHProbe->setPhSlope("?SLOPE,99.7,110.4,-0.89\r");
  assertTrue(pHProbe->slopeIsBad());
  pHProbe->clearCalibration();
  assertFalse(eeprom->getIgnoreBadPHSlope());
  assertFalse(pHProbe->slopeIsBad());
}

unittest(sendCalibrationRequest) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->sendCalibrationRequest();
  assertEqual("CAL,?\r", state->serialPort[1].dataOut);
  assertEqual("PH Calibration", pHProbe->getCalibrationResponse());
}

unittest(getCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  char buffer[17];
  pHProbe->setCalibration(0);
  pHProbe->getCalibration(buffer, sizeof(buffer));
  assertEqual("PH Calibra: 0 pt", buffer);
  pHProbe->setCalibration(3);
  pHProbe->getCalibration(buffer, sizeof(buffer));
  assertEqual("PH Calibra: 3 pt", buffer);
}

unittest(setTemperatureCompensation) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->setTemperatureCompensation(30.25);
  assertEqual("T,30.25\r", state->serialPort[1].dataOut);
  state->serialPort[1].dataOut = "";
  pHProbe->setTemperatureCompensation(100.25);
  assertEqual("T,20\r", state->serialPort[1].dataOut);
  state->serialPort[1].dataOut = "";
  pHProbe->setTemperatureCompensation(-1.25);
  assertEqual("T,20\r", state->serialPort[1].dataOut);
}

unittest(setLowpointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  // TODO: the following two lines are commented out in another branch
  eeprom->setIgnoreBadPHSlope(true);
  assertTrue(eeprom->getIgnoreBadPHSlope());
  assertEqual("", state->serialPort[0].dataOut);
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->setLowpointCalibration(10.875);
  assertEqual("PHProbe::setLowpointCalibration(10.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,low,10.875\r", state->serialPort[1].dataOut);
  // TODO: the following line is commented out in another branch
  assertFalse(eeprom->getIgnoreBadPHSlope());
}

unittest(setMidpointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  DataLogger::instance()->reset();
  assertFalse(DataLogger::instance()->getShouldWriteWarning());
  eeprom->setIgnoreBadPHSlope(true);
  assertTrue(eeprom->getIgnoreBadPHSlope());
  assertTrue(DataLogger::instance()->getShouldWriteWarning());
  DataLogger::instance()->reset();
  assertFalse(DataLogger::instance()->getShouldWriteWarning());
  assertEqual("", state->serialPort[1].dataOut);
  state->serialPort[0].dataOut = "";
  pHProbe->setMidpointCalibration(11.875);
  assertTrue(DataLogger::instance()->getShouldWriteWarning());
  assertEqual(
      "PHProbe::setMidpointCalibration(11.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,mid,11.875\r", state->serialPort[1].dataOut);
  assertFalse(eeprom->getIgnoreBadPHSlope());
}

unittest(settingMidpointClearsBadCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  eeprom->setIgnoreBadPHSlope(true);
  assertTrue(eeprom->getIgnoreBadPHSlope());
  pHProbe->setPhSlope("?SLOPE,-2.7,100.0,-0.50\r");
  assertTrue(pHProbe->slopeIsBad());
  pHProbe->setMidpointCalibration(11.875);
  assertFalse(eeprom->getIgnoreBadPHSlope());
  assertFalse(pHProbe->slopeIsBad());
}

unittest(setHighpointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  // TODO: the following two lines are commented out in another branch
  eeprom->setIgnoreBadPHSlope(true);
  assertTrue(eeprom->getIgnoreBadPHSlope());
  assertEqual("", state->serialPort[0].dataOut);
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->setHighpointCalibration(12.875);
  assertEqual("PHProbe::setHighpointCalibration(12.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,High,12.875\r", state->serialPort[1].dataOut);
  // TODO: the following line is commented out in another branch
  assertFalse(eeprom->getIgnoreBadPHSlope());
}

unittest(sendSlopeRequest) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->sendSlopeRequest();
  assertEqual("SLOPE,?\r", state->serialPort[1].dataOut);
  assertEqual("Requesting...", pHProbe->getSlopeResponse());
}

// this test assumes that earlier tests have run and that there is a slope available
unittest(getSlope) {
  GodmodeState *state = GODMODE();
  state->reset();
  state->serialPort[0].dataOut = "";
  pHProbe->setPhSlope();
  char buffer[20];
  pHProbe->getSlope(buffer, sizeof(buffer));
  assertEqual("99.7,100.3,-0.89", buffer);
  COUT(state->serialPort[0].dataOut.length());
  pHProbe->setPhSlope("?SLOPE,98.7,101.3,-0.89\r");
  COUT(state->serialPort[0].dataOut.length());
  state->serialPort[0].dataOut = "";
  pHProbe->getSlope(buffer, sizeof(buffer));
  assertEqual("98.7,101.3,-0.89", buffer);
}

unittest(getPh) {
  GodmodeState *state = GODMODE();
  state->serialPort[0].dataOut = "";
  state->reset();
  pHProbe->setPh(7.25);
  float pH = pHProbe->getPh();
  assertEqual(7.25, pH);
}

unittest_main()
