#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/EthernetServer_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PushingBox.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "TankController.h"
#include "UIState/PHCalibrationMid.h"

EthernetClient *pClient;
GodmodeState *state = GODMODE();
PushingBox *pPushingBox;
TankController *tc;
TempProbe_TC *tempProbe;
PHControl *controlSolenoid;

unittest_setup() {
  tc = TankController::instance();
  Ethernet.mockDHCP(IPAddress(192, 168, 1, 42));
  Ethernet_TC::instance(true);
  pPushingBox = PushingBox::instance();
  pPushingBox->setDeviceID("PushingBoxIdentifier");
  pClient = pPushingBox->getClient();
  tempProbe = TempProbe_TC::instance();
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
  // set tank id to 0, set time interval to 1 minute
  EEPROM_TC::instance()->setTankID(0);
  EEPROM_TC::instance()->setGoogleSheetInterval(1);

  tc->loop(false);
  delay(75 * 1000);  // a bit over one minute
  state->serialPort[0].dataOut = "";
  // Trigger SD logging and Serial (DataLogger_TC) and PushingBox
  tc->loop(false);
  char expected[] =
      "15:26 pH=7.000 temp= 0.00\r\n"
      "Set Tank ID in order to send data to PushingBox\r\n";
  assertEqual(expected, state->serialPort[0].dataOut);
}

unittest(SendData) {
  state->reset();
  EEPROM_TC::instance()->setTankID(99);
  TemperatureControl::instance()->setTargetTemperature(20.25);
  tempProbe->setTemperature(20.25, true);
  PHProbe::instance()->setPh(7.125);
  EthernetClient::startMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80,
                                  (const uint8_t *)"[PushingBox response]\r\n");
  assertFalse(pClient->connected());  // not yet connected!
  state->serialPort[0].dataOut = "";
  delay(120 * 1000);  // less than this seems to cause a crash!?
  // delay(60 * 20 * 1000);  // wait for 20 minutes to ensure we send again
  tc->loop(false);
  assertTrue(pClient->connected());
  assertNotNull(pClient->writeBuffer());
  deque<uint8_t> buffer = *(pClient->writeBuffer());
  String bufferResult;
  bool flag = false;
  for (int i = 0; i < buffer.size(); i++) {
    // Capture everything after the first 'G'
    flag = flag || buffer[i] == 'G';
    if (flag) {
      bufferResult += buffer[i];
    }
  }
  char expected1[] =
      "GET /pushingbox?devid=PushingBoxIdentifier&tankid=99&tempData=20.25&pHdata=7.125 HTTP/1.1\r\n"
      "Host: api.pushingbox.com\r\n"
      "Connection: close\r\n"
      "\r\n";
  assertEqual(expected1, bufferResult.c_str());
  char expected2[] =
      "15:25 pH=7.125 temp=20.25\r\n"
      "GET /pushingbox?devid=PushingBoxIdentifier&tankid=99&tempData=20.25&pHdata=7.125 HTTP/1.1\r\n"
      "attempting to connect to PushingBox...\r\n"
      "connected\r\n"
      "===== PushingBox response:\r\n"
      "[PushingBox response]\r\n"
      "===== end of PushingBox response\r\n";
  assertEqual(expected2, state->serialPort[0].dataOut);
}

unittest(inCalibration) {
  EEPROM_TC::instance()->setTankID(99);
  PHCalibrationMid *test = new PHCalibrationMid(tc, 3);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  EthernetClient::startMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());
  delay(60 * 20 * 1000);  // wait for 20 minutes to ensure we send again
  tc->loop(false);
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
  assertFalse(Ethernet_TC::instance(true)->getIsUsingDHCP());
  EthernetClient::startMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());
  delay(60 * 20 * 1000);  // wait for 20 minutes to ensure we still do not send
  tc->loop(false);
  assertFalse(pClient->connected());
}

unittest(NoDeviceID) {
  pPushingBox->setDeviceID("");
  EthernetClient::startMockServer(pPushingBox->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());
  delay(60 * 20 * 1000);  // wait for 20 minutes to ensure we still do not send
  tc->loop(false);
  assertFalse(pClient->connected());
}

unittest_main()
