#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DataLogger.h"
#include "DateTime_TC.h"
#include "PHControl.h"
#include "PHProbe.h"
#include "RemoteLogPusher.h"
#include "SD_TC.h"
#include "TC_util.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->reset();
  Ethernet.mockDHCP(IPAddress(192, 168, 1, 42));
  SD_TC::instance()->format();  // reset the remote log file
  DataLogger::instance()->reset();
  RemoteLogPusher::instance()->reset();
  TankController::instance();
}

unittest_teardown() {
}

unittest(singleton) {
  RemoteLogPusher* thing1 = RemoteLogPusher::instance();
  RemoteLogPusher* thing2 = RemoteLogPusher::instance();
  assertTrue(thing1 != nullptr);
  assertEqual(thing1, thing2);
}

unittest(loopSendsRequests) {
  TankController* tc = TankController::instance();
  RemoteLogPusher* pusher = RemoteLogPusher::instance();
  EthernetClient* pClient = pusher->getClient();
  RemoteLogPusher::instance()->setRemoteLogName("90A2DA807B76");

  // Set up the server to respond to the HEAD request
  assertTrue(Ethernet_TC::instance(true)->isConnectedToNetwork());
  EthernetClient::startMockServer(
    pusher->getServerDomain(), 
    (uint32_t)0, 
    80, 
    (const uint8_t *)"HTTP/1.1 404 Not Found\r\n"
      "Date: Mon, 21 Aug 2023 16:33:52 GMT\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
  );

  // We start the test with a fresh remote log file
  assertFalse(pusher->shouldSendHeadRequest());
  assertFalse(pusher->isReadyToPost());
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertFalse(pClient->connected());
  char buffer[100];
  SD_TC::instance()->getRemoteLogContents(buffer, sizeof(buffer), 0);
  assertEqual("", buffer);

  DataLogger::instance()->writeWarningSoon();
  tc->loop(false);  // write to data log
  assertTrue(pusher->basicShouldSendHeadRequest());
  assertFalse(pusher->shouldSendHeadRequest());
  tc->loop(false);  // write to remote log
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  SD_TC::instance()->getRemoteLogContents(buffer, sizeof(buffer), 0);
  buffer[7] = '\0';  // truncate the message
  assertEqual("Version", buffer);

  // After a start-up delay we send a HEAD request
  tc->loop(false);
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertTrue(pusher->basicShouldSendHeadRequest());
  assertFalse(pusher->shouldSendHeadRequest());
  assertFalse(pusher->isReadyToPost());
  delay(40000);
  assertTrue(pusher->shouldSendHeadRequest());
  tc->loop(false);  // Send HEAD request to server
  assertTrue(pClient->connected());
  assertEqual(PROCESS_HEAD_RESPONSE, pusher->getState());
  assertFalse(pusher->shouldSendHeadRequest());

  // The server responds with a 404 (file not found)
  tc->loop(false);  // handle PROCESS_HEAD_RESPONSE
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertFalse(pusher->isReadyToPost());

  pClient->stop();  // clears the readBuffer (but not the write buffer!?)
  assertFalse(pClient->connected());
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 80);
  EthernetClient::startMockServer(
    pusher->getServerDomain(), 
    (uint32_t)0, 
    80, 
    (const uint8_t *)"HTTP/1.1 200 OK\r\n"
      "Date: Tue, 12 Mar 2024 16:33:52 GMT\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
  );

  // After a brief delay we send the post request
  delay(4000);
  assertTrue(pusher->isReadyToPost());
  tc->loop(false);  // send POST request to server
  assertFalse(pusher->isReadyToPost());
  assertEqual(PROCESS_POST_RESPONSE, pusher->getState());

  // The server should respond with a 200 OK
  tc->loop(false);  // get response to POST
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertFalse(pusher->isReadyToPost());
  assertFalse(pusher->shouldSendHeadRequest());
  assertFalse(pClient->connected());

  // After a brief delay we could send another HEAD request
  delay(4000);
  assertTrue(pusher->shouldSendHeadRequest());

  // set up server for next HEAD request
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 80);
  EthernetClient::startMockServer(
    pusher->getServerDomain(), 
    (uint32_t)0, 
    80, 
    (const uint8_t*)"HTTP/1.1 200 OK\r\n"
                    "Date: Mon, 21 Aug 2023 16:33:54 GMT\r\n"
                    "Content-Length: 664\r\n"
                    "Content-Type: text/html\r\n"
                    "\r\n"
  );
  tc->loop(false);  // HEAD request is sent
  assertFalse(pusher->isReadyToPost());
  uint32_t localFileSize = SD_TC::instance()->getRemoteFileSize();
  SD_TC::instance()->updateRemoteLogFileSizeForTest();  // size to zero
  // SD_TC::instance()->writeRemoteLog("some data here");  // and '\n' is added for 15 bytes
  tc->loop(false);                               // "200 OK" is received
  assertFalse(pusher->shouldSendHeadRequest());  //
  assertFalse(pusher->isReadyToPost());          // because server has all 15 bytes
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 80);
}

unittest(noInternetConnectionWhenBubblerIsOn) {
  TankController* tc = TankController::instance();
  RemoteLogPusher* pusher = RemoteLogPusher::instance();
  assertFalse(pusher->basicShouldSendHeadRequest());
  EthernetClient* pClient = pusher->getClient();

  // Turn on the bubbler
  PHControl* controlSolenoid = PHControl::instance();
  PHProbe* pHProbe = PHProbe::instance();
  controlSolenoid->setBaseTargetPh(7.50);
  pHProbe->setPh(8.5);
  tc->loop(false);  // update the controls based on the current readings
  assertTrue(controlSolenoid->isOn());
  assertTrue(pusher->basicShouldSendHeadRequest());

  // Set up the server to respond to the HEAD request
  assertTrue(Ethernet_TC::instance(true)->isConnectedToNetwork());
  EthernetClient::startMockServer(
    pusher->getServerDomain(), 
    (uint32_t)0, 
    80, 
    (const uint8_t *)"HTTP/1.1 404 Not Found\r\n"
      "Date: Mon, 21 Aug 2023 16:33:52 GMT\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
  );

  // We start the test with a fresh remote log file
  DataLogger::instance()->writeWarningSoon();
  assertTrue(pusher->basicShouldSendHeadRequest());
  assertFalse(pusher->shouldSendHeadRequest());
  assertFalse(pusher->isReadyToPost());
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertFalse(pClient->connected());
  char buffer[100];
  SD_TC::instance()->getRemoteLogContents(buffer, sizeof(buffer), 0);
  buffer[7] = '\0';                // truncate the message
  assertEqual("Version", buffer);  // We have data to send to the server

  // Allow start-up delay to pass; we should not send a HEAD request
  tc->loop(false);
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertTrue(pusher->basicShouldSendHeadRequest());
  assertFalse(pusher->shouldSendHeadRequest());
  assertFalse(pusher->isReadyToPost());
  delay(40000);
  assertTrue(pusher->basicShouldSendHeadRequest());
  assertFalse(pusher->shouldSendHeadRequest());  // because bubbler is on
  tc->loop(false);
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertTrue(pusher->basicShouldSendHeadRequest());
  assertFalse(pClient->connected());

  // Allow time to pass to turn off bubbler
  delay(7500);
  assertFalse(pClient->connected());
  assertTrue(pusher->basicShouldSendHeadRequest());
  pHProbe->setPh(7.25);  // this also does a loop() call
  assertFalse(pusher->basicShouldSendHeadRequest());
  assertTrue(pClient->connected());
  assertEqual(PROCESS_HEAD_RESPONSE, pusher->getState());
  // from here on we are testing the normal case
}

unittest_main()
