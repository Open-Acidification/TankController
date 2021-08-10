#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/EthernetServer_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PushingBox.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "TankControllerLib.h"
#include "UIState/PHCalibrationMid.h"

GodmodeState *state = GODMODE();
PushingBox *pPushingBox;
TankControllerLib *tc;
TempProbe_TC *tempProbe;
PHControl *controlSolenoid;

unittest_setup() {
  tc = TankControllerLib::instance("PushingBoxIdentifier");
  pPushingBox = PushingBox::instance();
  tempProbe = TempProbe_TC::instance();
  controlSolenoid = PHControl::instance();
  DateTime_TC now(2021, 6, 8, 15, 25, 15);
  now.setAsCurrent();
  controlSolenoid->enablePID(false);
  controlSolenoid->setTargetPh(7.00);
  state->serialPort[1].dataIn = "7.00\r";  // the queue of data waiting to be read
  tc->serialEvent1();                      // fake interrupt to update the current pH reading
  tc->loop();                              // update the controls based on the current readings
  state->serialPort[0].dataOut = "";       // clear serial output
}

unittest(NoTankID) {
  // set tank id to 0, set time interval to 1 minute
  EEPROM_TC::instance()->setTankID(0);
  EEPROM_TC::instance()->setGoogleSheetInterval(1);

  tc->loop();
  delay(75 * 1000);  // a bit over one minute
  state->serialPort[0].dataOut = "";
  tc->loop();
  char expected[] =
      "15:26 pH=7.000 temp=-242.02\r\n"
      "Set Tank ID in order to send data to PushingBox\r\n";
  assertEqual(expected, state->serialPort[0].dataOut);
}

unittest(SendData) {
  state->reset();

  // set tank id
  EEPROM_TC::instance()->setTankID(99);

  // set temperature
  TemperatureControl::instance()->setTargetTemperature(20.25);
  tempProbe->setTemperature(20.25);
  tempProbe->setCorrection(0.0);
  for (int i = 0; i < 100; ++i) {
    delay(1000);
    tempProbe->getRunningAverage();
  }

  // set pH
  state->serialPort[1].dataIn = "7.125\r";  // the queue of data waiting to be read
  tc->serialEvent1();                       // fake interrupt
  EthernetClient::startMockServer(pPushingBox->getServer(), 80);
  assertEqual(0, pPushingBox->getClient()->writeBuffer().size());
  const uint8_t response[] = "[PushingBox response]\r\n";
  for (int i = 0; i < sizeof(response); ++i) {
    pPushingBox->getClient()->pushToReadBuffer(response[i]);
  }
  state->serialPort[0].dataOut = "";
  delay(60 * 1000);  // wait for one minute to ensure we send again
  tc->loop();
  deque<uint8_t> buffer = pPushingBox->getClient()->writeBuffer();
  String bufferResult;
  bool flag = false;
  for (int i = 0; i < buffer.size(); i++) {
    flag = flag || buffer[i] == 'G';
    if (flag) {
      bufferResult += buffer[i];
    }
  }
  char expected1[] =
      "GET /pushingbox?devid=PushingBoxIdentifier&tankid=99&tempData=20.26&pHdata=7.125 HTTP/1.1\r\n"
      "Host: api.pushingbox.com\r\n"
      "Connection: close\r\n"
      "\r\n";
  assertEqual(expected1, bufferResult.c_str());
  char expected2[] =
      "15:26 pH=7.125 temp=20.26\r\n"
      "GET /pushingbox?devid=PushingBoxIdentifier&tankid=99&tempData=20.26&pHdata=7.125 HTTP/1.1\r\n"
      "attempting to connect to PushingBox...\r\n"
      "connected\r\n"
      "===== PushingBox response:\r\n"
      "[PushingBox response]\r\n?"
      "===== end of PushingBox response\r\n";
  assertEqual(expected2, state->serialPort[0].dataOut);
  EthernetClient::stopMockServer(pPushingBox->getServer(), 80);
}

unittest(inCalibration) {
  pPushingBox->getClient()->stop();  // clears the writeBuffer and readBuffer

  // set tank id
  EEPROM_TC::instance()->setTankID(99);
  PHCalibrationMid *test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  EthernetClient::startMockServer(pPushingBox->getServer(), 80);
  assertEqual(0, pPushingBox->getClient()->writeBuffer().size());
  delay(60 * 20 * 1000);  // wait for 20 minutes to ensure we send again
  tc->loop();
  deque<uint8_t> buffer = pPushingBox->getClient()->writeBuffer();
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
unittest_main()
