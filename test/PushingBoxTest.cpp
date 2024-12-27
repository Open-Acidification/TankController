#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "EthernetServer_TC.h"
#include "PHControl.h"
#include "PushingBox.h"
#include "Serial_TC.h"
#include "TankController.h"
#include "ThermalControl.h"
#include "ThermalProbe_TC.h"
#include "UIState/PHCalibrationMid.h"

EthernetClient *pClient;
GodmodeState *state = GODMODE();
PushingBox *pPushingBox;
TankController *tc;
ThermalProbe_TC *thermalProbe;
PHControl *controlSolenoid;

unittest_setup() {
  state->reset();
  tc = TankController::instance();
  Ethernet.mockDHCP(IPAddress(192, 168, 1, 42));
  Ethernet_TC::instance(true);
  pPushingBox = PushingBox::instance();
  pPushingBox->setDeviceID("PushingBoxIdentifier");
  pPushingBox->resetNextSendTime();
  pClient = pPushingBox->getClient();
  thermalProbe = ThermalProbe_TC::instance();
  controlSolenoid = PHControl::instance();
  DateTime_TC now(2021, 6, 8, 15, 25, 15);
  now.setAsCurrent();
  controlSolenoid->enablePID(false);
  controlSolenoid->setBaseTargetPh(7.00);
  PHProbe::instance()->setPh(7.0);
  state->serialPort[0].dataOut = "";  // clear serial output
}

unittest_teardown() {
  if (pClient->writeBuffer()) {
    pClient->writeBuffer()->clear();
  }
  pClient->stop();  // clears the readBuffer (but not the write buffer!?)
  EthernetClient::stopMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80);
}

unittest(NoTankID) {
  // auto expected1 = "RemoteLogPusher: connection to oap.cs.wallawalla.edu failed";
  auto expected1 = "heater turned on at 1813 after 1813 ms";
  assertEqual(expected1, Serial_TC::instance()->getBuffer());
  Serial_TC::instance()->clearBuffer();
  // set tank id to 0, set time interval to 1 minute
  EEPROM_TC::instance()->setTankID(0);

  delay(30 * 1000);  // allow 30 seconds for time update
  tc->loop();
  auto expected2 = "GetTime: connected to oap.cs.wallawalla.edu";
  assertEqual(expected2, Serial_TC::instance()->getBuffer());
  Serial_TC::instance()->clearBuffer();
  tc->loop();
  delay(20 * 1000);  // allow 50 seconds (30 + 40) for RemoteLogPusher update
  tc->loop();        // Trigger SD logging and Serial (DataLogger)
  auto expected3 = "RemoteLogPusher: connection to oap.cs.wallawalla.edu failed";
  assertEqual(expected3, Serial_TC::instance()->getBuffer());
  Serial_TC::instance()->clearBuffer();
  delay(20 * 1000);  // allow 70 seconds (30 + 20 + 20) for PushingBox update
  tc->loop();        // Trigger PushingBox
  auto expected4 = "Set Tank ID in order to send data to PushingBox";
  assertEqual(expected4, Serial_TC::instance()->getBuffer());
  Serial_TC::instance()->clearBuffer();
}

unittest(SendData) {
  EEPROM_TC::instance()->setTankID(99);
  ThermalControl::instance()->setThermalTarget(20.25);
  thermalProbe->setTemperature(20.25, true);
  PHProbe::instance()->setPh(7.125);
  EthernetClient::startMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80,
                                  (const uint8_t *)"[PushingBox response]\r\n");
  assertFalse(pClient->connected());  // not yet connected!
  delay(60 * 1000);                   // allow for time update
  tc->loop();
  state->serialPort[0].dataOut = "";
  delay(10 * 1000);  // allow for PushingBox update
  tc->loop();
  char expected[] =
      "New info written to remote log\r\n"
      "GET /pushingbox?devid=PushingBoxIdentifier&tankid=99&tempData=20.25&pHdata=7.125 HTTP/1.1\r\n"
      "attempting to connect to PushingBox...\r\n"
      "connected\r\n"
      "===== PushingBox response:\r\n"
      "[PushingBox response]\r\n"
      "===== end of PushingBox response\r\n";
  assertEqual(expected, state->serialPort[0].dataOut);
}

unittest(inCalibration) {
  EEPROM_TC::instance()->setTankID(99);
  PHCalibrationMid *test = new PHCalibrationMid();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  EthernetClient::startMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());
  delay(60 * 20 * 1000);  // wait for 20 minutes to ensure we send again
  tc->loop();
  assertTrue(pClient->connected());
  assertNotNull(pClient->writeBuffer());
  deque<uint8_t> buffer = *(pClient->writeBuffer());
  String bufferResult;
  for (int i = 0; i < buffer.size(); i++) {
    bufferResult += buffer[i];
  }
  char expected1[] =
      "GET /pushingbox?devid=PushingBoxIdentifier&tankid=99&tempData=C&pHdata=C HTTP/1.1\r\n"
      "Host: api.pushingbox.com\r\n"
      "Connection: close\r\n"
      "\r\n";
  assertEqual(expected1, bufferResult.c_str());
}

unittest(without_DHCP) {
  Ethernet.mockDHCP(IPAddress((uint32_t)0));
  assertFalse(Ethernet_TC::instance(true)->isConnectedToNetwork());
  EthernetClient::startMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());
  delay(60 * 20 * 1000);  // wait for 20 minutes to ensure we still do not send
  tc->loop();
  assertFalse(pClient->connected());
}

unittest(NoDeviceID) {
  pPushingBox->setDeviceID("");
  EthernetClient::startMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());
  delay(60 * 20 * 1000);  // wait for 20 minutes to ensure we still do not send
  tc->loop();
  assertFalse(pClient->connected());
}

unittest_main()
