#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "AlertPusher.h"
#include "DateTime_TC.h"
#include "SD_TC.h"
#include "TC_util.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->reset();
  Ethernet.mockDHCP(IPAddress(192, 168, 1, 42));
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
  assertTrue(Ethernet_TC::instance(true)->isConnectedToNetwork());
  EthernetClient::startMockServer(pusher->getServerDomain(), (uint32_t)0, 80, (const uint8_t *)"HTTP/1.1 404 Not Found\r\n"
                                                   "Date: Mon, 21 Aug 2023 16:33:52 GMT\r\n"
                                                   "Content-Type: text/html\r\n"
                                                   "\r\n");

  // Requests are delayed 40 seconds from boot time
  assertFalse(pusher->getShouldSendHeadRequest());
  // assertFalse(pusher->getReadyToPost());
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  tc->loop(false);  // something is written to alert file
  char buffer[100];
  SD_TC::instance()->getAlert(buffer, sizeof(buffer), 0);
  assertEqual("", buffer);
  // assertFalse(pusher->getShouldSendHeadRequest());
  // pusher->pushSoon();
  assertTrue(pusher->getShouldSendHeadRequest());
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  // assertFalse(pusher->getReadyToPost());
  tc->loop(false);  // too soon; HEAD request is not sent

  // Send HEAD request to server and receive 404 Not Found
  delay(38000);
  tc->loop(false);              // write more alerts
  SD_TC::instance()->format();  // remove alerts that have been written
  assertTrue(pusher->getShouldSendHeadRequest());
  // assertFalse(pusher->getReadyToPost());
  delay(2000);
  assertTrue(pusher->getShouldSendHeadRequest());
  tc->loop(false);  // HEAD request is sent
  // assertFalse(pusher->getReadyToPost());
  tc->loop(false);  // "HTTP/1.1 404 Not Found" is received
  assertFalse(pusher->getShouldSendHeadRequest());
  // assertTrue(pusher->getReadyToPost());
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 80);

  // Send POST request to server and receive 200 OK
  EthernetClient::startMockServer(pusher->getServerDomain(), (uint32_t)0, 80,
                                  (const uint8_t*)"HTTP/1.1 200 OK\r\n\r\n");
  tc->loop(false);  // POST request is sent
  // assertFalse(pusher->getShouldSendHeadRequest());
  tc->loop(false);  // "HTTP/1.1 200 OK" is received
  assertTrue(pusher->getShouldSendHeadRequest());
  // assertFalse(pusher->getReadyToPost());
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 80);

  // Send HEAD request to server and receive 200 OK
  EthernetClient::startMockServer(pusher->getServerDomain(), (uint32_t)0, 80, (const uint8_t*)"HTTP/1.1 200 OK\r\n"
                                  "Date: Mon, 21 Aug 2023 16:33:54 GMT\r\n"
                                  "Content-Length: 15"
                                  "Content-Type: text/html\r\n"
                                  "\r\n");
  tc->loop(false);  // HEAD request is sent
  // assertFalse(pusher->getReadyToPost());
  SD_TC::instance()->format();                      // wipe alert log file
  SD_TC::instance()->updateAlertFileSizeForTest();  // size to zero
  SD_TC::instance()->writeAlert("some data here");  // and '\n' is added for 15 bytes
  tc->loop(false);                                  // "200 OK" is received
  assertFalse(pusher->getShouldSendHeadRequest());  //
  // assertFalse(pusher->getReadyToPost());            // because server has all 15 bytes
  assertEqual(PROCESS_HEAD_RESPONSE, pusher->getState());
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, 80);
}

unittest_main()
