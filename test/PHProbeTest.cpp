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

// tests getPH() as well
unittest(serialEvent1) {
  GodmodeState *state = GODMODE();
  state->reset();
  PHProbe *pPHProbe = PHProbe::instance();
  assertEqual(0, pPHProbe->getPH());
  GODMODE()->serialPort[1].dataIn = "7.75\r";  // the queue of data waiting to be read
  TankControllerLib *pTC = TankControllerLib::instance();
  state->serialPort[0].dataOut = "";
  assertEqual("", state->serialPort[0].dataOut);
  pTC->serialEvent1();
  assertEqual("pH = 7.750\r\n", state->serialPort[0].dataOut);
  assertEqual(7.75, PHProbe::instance()->getPH());
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

unittest_main()
