#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/EthernetServer_TC.h"
#include "Devices/PushingBox.h"

unittest(Client_Write) {
  byte server[4] = { 64, 233, 187, 99 }; // Google
  PushingBox *pPushingBox = PushingBox::instance();
  int tankId = 99;
  float temperature = 20.12;
  float pH = 7.125;
  EthernetClient client;
  EthernetClient::startMockServer(server, 80);
  assertEqual(0, client.writeBuffer().size());
  pPushingBox->sendData(tankId, temperature, pH, client);
  deque<uint8_t> buffer = client.writeBuffer();
  String bufferResult;
  for (int i = 0; i < buffer.size(); i++) {
    bufferResult += buffer[i];
  }
  assertEqual("GET /pushingbox?devid=v172D35C152EDA6C&tankid=99&tempData=20.12&pHdata=7.125 HTTP/1.1", bufferResult.c_str());
  EthernetClient::stopMockServer(server, 80);
}

unittest_main()
