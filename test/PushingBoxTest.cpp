#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/EthernetServer_TC.h"
#include "Devices/PushingBox.h"

unittest(Client_Write) {
  GodmodeState *state = GODMODE();
  state->reset();
  state->serialPort[0].dataOut = "";
  PushingBox *pPushingBox = PushingBox::instance();
  int tankId = 99;
  float temperature = 20.12;
  float pH = 7.125;
  EthernetClient::startMockServer(pPushingBox->getServer(), 80);
  assertEqual(0, pPushingBox->getClient()->writeBuffer().size());
  pPushingBox->getClient()->pushToReadBuffer('A');
  pPushingBox->sendDataHTTPRequest(tankId, temperature, pH);
  deque<uint8_t> buffer = pPushingBox->getClient()->writeBuffer();
  String bufferResult;
  for (int i = 0; i < buffer.size(); i++) {
    bufferResult += buffer[i];
  }
  assertEqual("GET /pushingbox?devid=v172D35C152EDA6C&tankid=99&tempData=20.12&pHdata=7.125 HTTP/1.1\r\n", bufferResult.c_str());
  assertEqual("", state->serialPort[0].dataOut);
  EthernetClient::stopMockServer(pPushingBox->getServer(), 80);
}

unittest_main()
