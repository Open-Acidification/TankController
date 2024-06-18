#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "AlertPusher.h"
#include "DateTime_TC.h"
#include "PHControl.h"
#include "PHProbe.h"
#include "SD_TC.h"
#include "TC_util.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->reset();
  Ethernet.mockDHCP(IPAddress(192, 168, 1, 42));
  SD_TC::instance()->format();  // reset the alert file
}

unittest_teardown() {
}

unittest(singleton) {
  AlertPusher* thing1 = AlertPusher::instance();
  AlertPusher* thing2 = AlertPusher::instance();
  assertTrue(thing1 != nullptr);
  assertEqual(thing1, thing2);
}

unittest(loopSendsRequests) {
  TankController* tc = TankController::instance();
  AlertPusher* pusher = AlertPusher::instance();
  EthernetClient* pClient = pusher->getClient();

  assertTrue(Ethernet_TC::instance(true)->isConnectedToNetwork());
  EthernetClient::startMockServer(
    pusher->getServerDomain(), 
    (uint32_t)0, 
    8080, 
    (const uint8_t *)"HTTP/1.1 404 Not Found\r\n"
      "Date: Mon, 21 Aug 2023 16:33:52 GMT\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
  );

  // We start the test with a fresh alert file
  assertFalse(pusher->shouldSendHeadRequest());
  assertFalse(pusher->isReadyToPost());
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertFalse(pClient->connected());
  char buffer[100];
  SD_TC::instance()->getAlert(buffer, sizeof(buffer), 0);
  assertEqual("", buffer);

  // during a loop we write to the alert file
  tc->loop(false);
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  SD_TC::instance()->getAlert(buffer, sizeof(buffer), 0);
  buffer[7] = '\0';  // truncate the message
  assertEqual("Version", buffer);

  // After a start-up delay we send a HEAD request
  tc->loop(false);
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
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
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 8080);
  EthernetClient::startMockServer(
    pusher->getServerDomain(), 
    (uint32_t)0, 
    8080, 
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

  // After a brief delay we are ready to send another HEAD request
  delay(4000);
  assertTrue(pusher->shouldSendHeadRequest());

  // set up server for next HEAD request
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 8080);
  EthernetClient::startMockServer(
    pusher->getServerDomain(), 
    (uint32_t)0, 
    8080, 
    (const uint8_t*)"HTTP/1.1 200 OK\r\n"
                    "Date: Mon, 21 Aug 2023 16:33:54 GMT\r\n"
                    "Content-Length: 664\r\n"
                    "Content-Type: text/html\r\n"
                    "\r\n"
  );
  tc->loop(false);  // HEAD request is sent
  assertFalse(pusher->isReadyToPost());
  uint32_t localFileSize = SD_TC::instance()->getAlertFileSize();
  SD_TC::instance()->updateAlertFileSizeForTest();  // size to zero
  SD_TC::instance()->writeAlert("some data here");  // and '\n' is added for 15 bytes
  tc->loop(false);                                  // "200 OK" is received
  assertFalse(pusher->shouldSendHeadRequest());     //
  assertFalse(pusher->isReadyToPost());             // because server has all 15 bytes
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 8080);
}

unittest(noInternetConnectionWhenBubblerIsOn) {
  TankController* tc = TankController::instance();
  PHControl* controlSolenoid = PHControl::instance();
  PHProbe* pHProbe = PHProbe::instance();
  AlertPusher* pusher = AlertPusher::instance();

  // Turn on the bubbler
  controlSolenoid->setBaseTargetPh(7.50);
  pHProbe->setPh(8.5);
  // tc->loop(false);  // update the controls based on the current readings
  // assertTrue(controlSolenoid->isOn());

  // Set up the server to respond to the HEAD request
  assertTrue(Ethernet_TC::instance(true)->isConnectedToNetwork());
  EthernetClient::startMockServer(
    pusher->getServerDomain(), 
    (uint32_t)0, 
    8080, 
    (const uint8_t *)"HTTP/1.1 404 Not Found\r\n"
      "Date: Mon, 21 Aug 2023 16:33:52 GMT\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
  );

  // We start the test with a fresh alert file
  assertFalse(pusher->shouldSendHeadRequest());
  assertFalse(pusher->isReadyToPost());
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  SD_TC::instance()->format();  // reset the alert file
  char buffer[100];
  SD_TC::instance()->getAlert(buffer, sizeof(buffer), 0);
  assertEqual("", buffer);

  // during a loop we write to the alert file
  tc->loop(false);
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  SD_TC::instance()->getAlert(buffer, sizeof(buffer), 0);
  buffer[7] = '\0';  // truncate the message
  assertEqual("Version", buffer);

  // After a start-up delay we send a HEAD request
  tc->loop(false);
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertFalse(pusher->shouldSendHeadRequest());
  assertFalse(pusher->isReadyToPost());
  delay(40000);
  assertTrue(pusher->shouldSendHeadRequest());
  tc->loop(false);  // Send HEAD request to server
  assertEqual(PROCESS_HEAD_RESPONSE, pusher->getState());
  assertFalse(pusher->shouldSendHeadRequest());


  // Turn off the bubbler
  delay(7500);
  pHProbe->setPh(7.25);
  tc->loop(false);  // update the controls based on the current readings
  assertFalse(controlSolenoid->isOn());
}

unittest_main()
