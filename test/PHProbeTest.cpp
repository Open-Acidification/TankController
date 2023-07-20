#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/PHProbe.h"
#include "TC_util.h"
#include "TankController.h"

unittest(singleton) {
  PHProbe *singleton1 = PHProbe::instance();
  PHProbe *singleton2 = PHProbe::instance();
  assertEqual(singleton1, singleton2);
}

unittest(constructor) {
  assertEqual("*OK,0\rC,1\r", GODMODE()->serialPort[1].dataOut);
}

// tests getPh() and getSlopeResponse as well
unittest(serialEvent1) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankController *tc = TankController::instance();
  state->serialPort[0].dataOut = "";
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe *pPHProbe = PHProbe::instance();  // the constructor writes data to the serial port
  tc->serialEvent1();                       // fake interrupt
  assertEqual("", pPHProbe->getCalibrationResponse());
  assertEqual(0, pPHProbe->getPh());
  assertEqual("", pPHProbe->getSlopeResponse());
  pPHProbe->setCalibration(2);
  pPHProbe->setPh(7.125);
  pPHProbe->setPhSlope();
  assertEqual("2 pt calibrated", pPHProbe->getCalibrationResponse());
  assertEqual(7.125, pPHProbe->getPh());
  assertEqual("99.7,100.3,-0.89", pPHProbe->getSlopeResponse());
}

unittest(clearCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe::instance()->clearCalibration();
  assertEqual("Cal,clear\r", state->serialPort[1].dataOut);
}

unittest(sendCalibrationRequest) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe::instance()->sendCalibrationRequest();
  assertEqual("CAL,?\r", state->serialPort[1].dataOut);
  assertEqual("Requesting...", PHProbe::instance()->getCalibrationResponse());
}

unittest(getCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankController *tc = TankController::instance();
  PHProbe *pPHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[1].dataOut);
  char buffer[17];
  pPHProbe->setCalibration(0);
  pPHProbe->getCalibration(buffer, sizeof(buffer));
  assertEqual("0 pt calibrated", buffer);
  pPHProbe->setCalibration(3);
  pPHProbe->getCalibration(buffer, sizeof(buffer));
  assertEqual("3 pt calibrated", buffer);
}

unittest(setTemperatureCompensation) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe *pPHProbe = PHProbe::instance();
  pPHProbe->setTemperatureCompensation(30.25);
  assertEqual("T,30.25\r", state->serialPort[1].dataOut);
  state->serialPort[1].dataOut = "";
  pPHProbe->setTemperatureCompensation(100.25);
  assertEqual("T,20\r", state->serialPort[1].dataOut);
  state->serialPort[1].dataOut = "";
  pPHProbe->setTemperatureCompensation(-1.25);
  assertEqual("T,20\r", state->serialPort[1].dataOut);
}

unittest(setLowpointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe *pPHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[0].dataOut);
  assertEqual("", state->serialPort[1].dataOut);
  pPHProbe->setLowpointCalibration(10.875);
  assertEqual("PHProbe::setLowpointCalibration(10.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,low,10.875\r", state->serialPort[1].dataOut);
}

unittest(setMidpointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe *pPHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[0].dataOut);
  assertEqual("", state->serialPort[1].dataOut);
  pPHProbe->setMidpointCalibration(11.875);
  assertEqual("PHProbe::setMidpointCalibration(11.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,mid,11.875\r", state->serialPort[1].dataOut);
}

unittest(setHighpointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe *pPHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[0].dataOut);
  assertEqual("", state->serialPort[1].dataOut);
  pPHProbe->setHighpointCalibration(12.875);
  assertEqual("PHProbe::setHighpointCalibration(12.875)\r\n", state->serialPort[0].dataOut);
  assertEqual("Cal,High,12.875\r", state->serialPort[1].dataOut);
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
  PHProbe *pPHProbe = PHProbe::instance();
  pPHProbe->setPhSlope();
  char buffer[20];
  pPHProbe->getSlope(buffer, sizeof(buffer));
  assertEqual("99.7,100.3,-0.89", buffer);
  COUT(state->serialPort[0].dataOut.length());
  pPHProbe->setPhSlope("?SLOPE,98.7,101.3,-0.89\r");
  COUT(state->serialPort[0].dataOut.length());
  state->serialPort[0].dataOut = "";
  pPHProbe->getSlope(buffer, sizeof(buffer));
  assertEqual("98.7,101.3,-0.89", buffer);
}

unittest(getPh) {
  GodmodeState *state = GODMODE();
  state->serialPort[0].dataOut = "";
  state->reset();
  PHProbe::instance()->setPh(7.25);
  PHProbe *pPHProbe = PHProbe::instance();
  float pH = pPHProbe->getPh();
  assertEqual(7.25, pH);
}

unittest_main()
