#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/EEPROM_TC.h"
#include "Devices/EthernetServer_TC.h"
#include "Devices/PushingBox.h"
#include "TankControllerLib.h"
#include "Devices/TempProbe_TC.h"

unittest(pushingbox) {
  GodmodeState *state = GODMODE();
  state->reset();
  state->serialPort[0].dataOut = "";
  PushingBox *pPushingBox = PushingBox::instance();
  TankControllerLib *pTC = TankControllerLib::instance();
  TempProbe_TC *tempProbe = TempProbe_TC::instance();

  // set tank id
  EEPROM_TC::instance()->setTankID(99);

  // set temperature
  tempProbe->setTemperature(20.25);
  tempProbe->setCorrection(0.0);
  for (int i = 0; i < 100; ++i) {
    delay(1000);
    tempProbe->getRunningAverage();
  }

  // set pH
  state->serialPort[1].dataIn = "7.125\r";  // the queue of data waiting to be read
  pTC->serialEvent1();                    // fake interrupt
  
  EthernetClient::startMockServer(pPushingBox->getServer(), 80);
  assertEqual(0, pPushingBox->getClient()->writeBuffer().size());
  pPushingBox->getClient()->pushToReadBuffer('A');
  state->serialPort[0].dataOut = "";
  pTC->loop();
  deque<uint8_t> buffer = pPushingBox->getClient()->writeBuffer();
  String bufferResult;
  for (int i = 0; i < buffer.size(); i++) {
    bufferResult += buffer[i];
  }
  char expected1[] = "GET /pushingbox?devid=v172D35C152EDA6C&tankid=99&tempData=20.26&pHdata=7.125 HTTP/1.1\r\n"
                    "Host: api.pushingbox.com\r\n" 
                    "Connection: close\r\n" 
                    "\r\n";

  assertEqual(expected1, bufferResult.c_str());
  char expected2[] = "PushingBox: Ethernet client not ready!?\r\n"
                      "attepting to connect to pushing box...\r\n"
                      "connected\r\nA";
  assertEqual(expected2, state->serialPort[0].dataOut);
  EthernetClient::stopMockServer(pPushingBox->getServer(), 80);
}

unittest_main()
