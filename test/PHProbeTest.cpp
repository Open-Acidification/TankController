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
unittest(probeData) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[0].dataOut);
  PHProbe *pPHProbe = PHProbe::instance();
  assertEqual(0, pPHProbe->getPH());
  pPHProbe->probeData("7.250");
  assertEqual(7.25, pPHProbe->getPH());
  assertEqual("pH = 7.250\r\n", state->serialPort[0].dataOut);
}

unittest(serialEvent1) {
  GODMODE()->serialPort[1].dataIn = "7.75\r";  // the queue of data waiting to be read
  TankControllerLib::instance()->serialEvent1();
  assertEqual(7.75, PHProbe::instance()->getPH());
}

unittest_main()
