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
  PHProbe *pPHProbe = PHProbe::instance();  // the constructor writes data to the serial port
  tc->serialEvent1();                       // fake interrupt
  assertEqual(0, pPHProbe->getPh());
  assertEqual("", pPHProbe->getSlopeResponse());
  GODMODE()->serialPort[1].dataIn = "7.125\r?SLOPE,99.7,100.3,-0.89\r";  // the queue of data waiting to be read
  tc->serialEvent1();                                                    // fake interrupt
  assertEqual("99.7,100.3,-0.89", pPHProbe->getSlopeResponse());
  assertEqual(7.125, pPHProbe->getPh());
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
  state->serialPort[0].dataOut = "";
  PHProbe::instance()->sendSlopeRequest();
  assertEqual("SLOPE,?\r", GODMODE()->serialPort[1].dataOut);
}

// this test assumes that earlier tests have run and that there is a slope available
unittest(getSlope) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankController *tc = TankController::instance();
  state->serialPort[0].dataOut = "";
  PHProbe *pPHProbe = PHProbe::instance();
  GODMODE()->serialPort[1].dataIn = "?SLOPE,99.7,100.3,-0.89\r";  // the queue of data waiting to be read
  tc->serialEvent1();                                             // fake interrupt
  char buffer[20];
  pPHProbe->getSlope(buffer, sizeof(buffer));
  assertEqual("99.7,100.3,-0.89", buffer);
  COUT(state->serialPort[0].dataOut.length());
  GODMODE()->serialPort[1].dataIn = "?SLOPE,98.7,101.3,-0.89\r";  // the answer to getSlop() waiting to be read
  tc->serialEvent1();                                             // fake interrupt
  COUT(state->serialPort[0].dataOut.length());
  state->serialPort[0].dataOut = "";
  pPHProbe->getSlope(buffer, sizeof(buffer));
  assertEqual("98.7,101.3,-0.89", buffer);
}

unittest(getPh) {
  GodmodeState *state = GODMODE();
  TankController *tc = TankController::instance();
  state->serialPort[0].dataOut = "";
  state->reset();
  state->serialPort[1].dataIn = "7.25\r";  // the queue of data waiting to be read
  tc->serialEvent1();                      // fake interrupt
  PHProbe *pPHProbe = PHProbe::instance();
  float pH = pPHProbe->getPh();
  assertEqual(7.25, pH);
}

unittest(clearCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe::instance()->clearCalibration();
  assertEqual("Cal,clear\r", GODMODE()->serialPort[1].dataOut);
}

unittest_main()
