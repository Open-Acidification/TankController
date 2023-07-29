#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/PHProbe.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"
#include "TankController.h"

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
  GodmodeState *state = GODMODE();
  state->reset();
  TankController *tc = TankController::instance();
  state->serialPort[0].dataOut = "";
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe *pHProbe = PHProbe::instance();  // the constructor writes data to the serial port
  tc->serialEvent1();                      // fake interrupt
  assertEqual("", pHProbe->getCalibrationResponse());
  assertEqual(0, pHProbe->getPh());
  assertEqual("Requesting...", pHProbe->getSlopeResponse());
  pHProbe->setCalibration(2);
  pHProbe->setPh(7.125);
  pHProbe->setPhSlope();
  assertEqual("PH Calibra: 2 pt", pHProbe->getCalibrationResponse());
  assertEqual(7.125, pHProbe->getPh());
  assertEqual("99.7,100.3,-0.89", pHProbe->getSlopeResponse());
}

unittest(serialEvent1CatchBadSlope) {
  TankController *tc = TankController::instance();
  PHProbe *pHProbe = PHProbe::instance();
  tc->setIgnoreBadPHSlope(true);
  assertTrue(tc->getIgnoreBadPHSlope());
  tc->serialEvent1();
  assertFalse(pHProbe->slopeIsBad());

  // Bad slope
  pHProbe->setPhSlope("?SLOPE,-2.7,101.3\r");
  assertTrue(pHProbe->slopeIsBad());
  assertTrue(tc->getIgnoreBadPHSlope());
  assertEqual("BAD CALIBRATION? pH slopes are more than 5\% from ideal", Serial_TC::instance()->buffer);

  // Good slope
  pHProbe->setPhSlope();
  assertFalse(pHProbe->slopeIsBad());
  assertFalse(tc->getIgnoreBadPHSlope());
  assertEqual("pH slopes are within 5\% of ideal", Serial_TC::instance()->buffer);

  // Bad slope
  pHProbe->setPhSlope("?SLOPE,98.7,107.2,-0.89\r");
  assertTrue(pHProbe->slopeIsBad());
  assertEqual("BAD CALIBRATION? pH slopes are more than 5\% from ideal", Serial_TC::instance()->buffer);
}

unittest(clearCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankController::instance()->setIgnoreBadPHSlope(true);
  assertTrue(TankController::instance()->getIgnoreBadPHSlope());
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe::instance()->clearCalibration();
  assertEqual("Cal,clear\r", state->serialPort[1].dataOut);
  assertFalse(TankController::instance()->getIgnoreBadPHSlope());
  assertFalse(PHProbe::instance()->slopeIsBad());
}

unittest(clearBadCalibration) {
  GodmodeState *state = GODMODE();
  PHProbe *pHProbe = PHProbe::instance();
  state->reset();
  TankController::instance()->setIgnoreBadPHSlope(true);
  assertTrue(TankController::instance()->getIgnoreBadPHSlope());
  pHProbe->setPhSlope("?SLOPE,99.7,110.4,-0.89\r");
  assertTrue(pHProbe->slopeIsBad());
  pHProbe->clearCalibration();
  assertFalse(TankController::instance()->getIgnoreBadPHSlope());
  assertFalse(pHProbe->slopeIsBad());
}

unittest(sendCalibrationRequest) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe::instance()->sendCalibrationRequest();
  assertEqual("CAL,?\r", state->serialPort[1].dataOut);
  assertEqual("PH Calibration", PHProbe::instance()->getCalibrationResponse());
}

unittest(getCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankController *tc = TankController::instance();
  PHProbe *pHProbe = PHProbe::instance();
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
  PHProbe *pHProbe = PHProbe::instance();
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
  TankController::instance()->setIgnoreBadPHSlope(true);
  assertTrue(TankController::instance()->getIgnoreBadPHSlope());
  PHProbe *pHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[0].dataOut);
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->setLowpointCalibration(10.875);
  assertEqual("PHProbe::setLowpointCalibration(10.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,low,10.875\r", state->serialPort[1].dataOut);
  assertFalse(TankController::instance()->getIgnoreBadPHSlope());
}

unittest(setMidpointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankController::instance()->setIgnoreBadPHSlope(true);
  assertTrue(TankController::instance()->getIgnoreBadPHSlope());
  PHProbe *pHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[0].dataOut);
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->setMidpointCalibration(11.875);
  assertEqual("PHProbe::setMidpointCalibration(11.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,mid,11.875\r", state->serialPort[1].dataOut);
  assertFalse(TankController::instance()->getIgnoreBadPHSlope());
}

unittest(settingMidpointClearsBadCalibration) {
  GodmodeState *state = GODMODE();
  PHProbe *pHProbe = PHProbe::instance();
  state->reset();
  TankController::instance()->setIgnoreBadPHSlope(true);
  assertTrue(TankController::instance()->getIgnoreBadPHSlope());
  pHProbe->setPhSlope("?SLOPE,-2.7,100.0,-0.50\r");
  assertTrue(pHProbe->slopeIsBad());
  pHProbe->setMidpointCalibration(11.875);
  assertFalse(TankController::instance()->getIgnoreBadPHSlope());
  assertFalse(pHProbe->slopeIsBad());
}

unittest(setHighpointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankController::instance()->setIgnoreBadPHSlope(true);
  assertTrue(TankController::instance()->getIgnoreBadPHSlope());
  PHProbe *pHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[0].dataOut);
  assertEqual("", state->serialPort[1].dataOut);
  pHProbe->setHighpointCalibration(12.875);
  assertEqual("PHProbe::setHighpointCalibration(12.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,High,12.875\r", state->serialPort[1].dataOut);
  assertFalse(TankController::instance()->getIgnoreBadPHSlope());
}

unittest(sendSlopeRequest) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe::instance()->sendSlopeRequest();
  assertEqual("SLOPE,?\r", state->serialPort[1].dataOut);
  assertEqual("Requesting...", PHProbe::instance()->getSlopeResponse());
}

// this test assumes that earlier tests have run and that there is a slope available
unittest(getSlope) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankController *tc = TankController::instance();
  state->serialPort[0].dataOut = "";
  PHProbe *pHProbe = PHProbe::instance();
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
  PHProbe::instance()->setPh(7.25);
  PHProbe *pHProbe = PHProbe::instance();
  float pH = pHProbe->getPh();
  assertEqual(7.25, pH);
}

unittest_main()
