#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/PHProbe.h"
#include "TankControllerLib.h"

unittest(singleton) {
  PHProbe *singleton1 = PHProbe::instance();
  PHProbe *singleton2 = PHProbe::instance();
  assertEqual(singleton1, singleton2);
}

unittest(constructor) {
  assertEqual("*OK,0\rC,1\r", GODMODE()->serialPort[1].dataOut);
}

// tests getPH() and getSlopeResponse as well
unittest(serialEvent1) {
  GodmodeState *state = GODMODE();
  state->reset();
  TankControllerLib *pTC = TankControllerLib::instance();
  PHProbe *pPHProbe = PHProbe::instance();
  pTC->serialEvent1();
  assertEqual(0, pPHProbe->getPH());
  assertEqual("", pPHProbe->getSlopeResponse());
  GODMODE()->serialPort[1].dataIn = "7.75\r?Slope,99.7,100.3,-0.89\r";  // the queue of data waiting to be read
  pTC->serialEvent1();
  assertEqual("?Slope,99.7,100.3,-0.89\r", pPHProbe->getSlopeResponse());
  assertEqual(7.75, pPHProbe->getPH());
}

unittest(setTemperatureCompensation) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[1].dataOut);
  PHProbe *pPHProbe = PHProbe::instance();
  pPHProbe->setTemperatureCompensation(30.25);
  assertEqual("T,30.25\r", GODMODE()->serialPort[1].dataOut);
  state->serialPort[1].dataOut = "";
  pPHProbe->setTemperatureCompensation(100.25);
  assertEqual("T,20\r", GODMODE()->serialPort[1].dataOut);
  state->serialPort[1].dataOut = "";
  pPHProbe->setTemperatureCompensation(-1.25);
  assertEqual("T,20\r", GODMODE()->serialPort[1].dataOut);
}

unittest(onePointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe *pPHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[1].dataOut);
  pPHProbe->onePointCalibration(10.875);
  assertEqual("Cal,mid,10.875\r", state->serialPort[1].dataOut);
}

unittest(twoPointCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe *pPHProbe = PHProbe::instance();
  assertEqual("", state->serialPort[1].dataOut);
  pPHProbe->twoPointCalibration(10.875, 11.875);
  assertEqual("Cal,mid,11.875\rCal,low,10.875\r", state->serialPort[1].dataOut);
}

unittest(getSlope) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe *pPHProbe = PHProbe::instance();
  GODMODE()->serialPort[1].dataIn = "?Slope,99.7,100.3,-0.89\r";  // the answer to getSlop() waiting to be read
  String slope = pPHProbe->getSlope();
  assertEqual("99.7,100.3,-0.89 ", slope);
  assertEqual("Raw String: \r\n?Slope,99.7,100.3,-0.89\r\r\n\r\nCalibration Slope: \r\n99.7,100.3,-0.89 \r\n\r\n",
              state->serialPort[0].dataOut);
}

unittest(getPhReading) {
  GodmodeState *state = GODMODE();
  state->reset();
  GODMODE()->serialPort[1].dataIn = "7.25";  // the queue of data waiting to be read
  PHProbe *pPHProbe = PHProbe::instance();
  double pH = pPHProbe->getPhReading();
  assertEqual(7.25, pH);
  assertEqual("pH = 7.250\r\n", state->serialPort[0].dataOut);
}

unittest(clearCalibration) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe *pPHProbe = PHProbe::instance();
  pPHProbe->onePointCalibration(10.875);
  GODMODE()->serialPort[1].dataIn = "?Cal,1\r";  // the answer to getSlop() waiting to be read
  Serial1.print("Cal,?\r");                      // send that string to the Atlas Scientific product
  String string = Serial1.readStringUntil(13);   // read the string until we see a <CR>
  assertEqual("?Cal,1\r", string);
  pPHProbe->clearCalibration();
  GODMODE()->serialPort[1].dataIn = "?Cal,0\r";  // the answer to getSlop() waiting to be read
  Serial1.print("Cal,?\r");                      // send that string to the Atlas Scientific product
  String string2 = Serial1.readStringUntil(13);  // read the string until we see a <CR>
  assertEqual("?Cal,0\r", string2);
}

unittest_main()
