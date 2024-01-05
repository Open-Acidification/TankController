#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "AlertPusher.h"
#include "DateTime_TC.h"
#include "PHProbe.h"
#include "SD_TC.h"
#include "TC_util.h"
#include "TankController.h"

char header[1000] =
    "Version\tTank ID\tSeverity\tDate Time\tMessage\tTemperature "
    "Target\tTemperature Mean\tTemperature Std Dev\tpH Target\tpH\t"
    "Uptime\tMAC Address\tpH Slope\t"
    "Ignoring Bad pH Calibration\tTemperature Correction\tIgnoring Bad "
    "Temperature Calibration\tHeat (1) or Chill (0)\tKD\tKI\tKP\t"
    "pH Flat (0) Ramp (1) Sine (2)\tpH Target\t"
    "pH Ramp Start Time\tpH Ramp End Time\tpH Ramp Start Value\t"
    "pH Sine Start Time\tpH Sine Period\tpH Sine Amplitude\t"
    "Temperature Flat (0) Ramp (1) Sine (2)\tTemperature Target\t"
    "Temperature Ramp Start Time\tTemperature Ramp End Time\t"
    "Temperature Ramp Start Value\tTemperature Sine Start Time\t"
    "Temperature Sine Period\tTemperature Sine Amplitude\t"
    "Google Sheet Interval\n";

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
  EthernetClient::startMockServer(pusher->getServerDomain(), (uint32_t)0, OAP_SERVER_PORT, (const uint8_t *)"HTTP/1.1 404 Not Found\r\n"
                                                   "Date: Mon, 21 Aug 2023 16:33:52 GMT\r\n"
                                                   "Content-Type: text/html\r\n"
                                                   "\r\n");

  // Requests are delayed 40 seconds from boot time
  assertFalse(pusher->getShouldSendHeadRequest());
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  tc->loop(false);
  PHProbe::instance()->setPhSlope("?SLOPE,99.11,100.11\r");  // triggers a write to alert file
  tc->loop(false);
  char buffer[1000];
  SD_TC::instance()->getAlert(buffer, sizeof(buffer), 0);
  string fileContents(buffer);
  assertTrue(fileContents.find("99.11,100.11") > 0);  // warning was sent
  assertTrue(pusher->getShouldSendHeadRequest());
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertFalse(pusher->getReadyToPost());
  tc->loop(false);  // too soon; HEAD request is not sent

  // Send HEAD request to server and receive 404 Not Found
  delay(38000);
  tc->loop(false);              // write more alerts
  SD_TC::instance()->format();  // remove alerts that have been written
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertTrue(pusher->getShouldSendHeadRequest());
  assertFalse(pusher->getReadyToPost());
  delay(2000);
  tc->loop(false);  // HEAD request is sent
  assertFalse(pusher->getShouldSendHeadRequest());
  assertEqual(PROCESS_HEAD_RESPONSE, pusher->getState());
  assertFalse(pusher->getReadyToPost());
  tc->loop(false);  // "HTTP/1.1 404 Not Found" is received
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertFalse(pusher->getShouldSendHeadRequest());
  assertTrue(pusher->getReadyToPost());
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, OAP_SERVER_PORT);

  // Send POST request to server and receive 200 OK
  EthernetClient::startMockServer(pusher->getServerDomain(), (uint32_t)0, OAP_SERVER_PORT,
                                  (const uint8_t*)"HTTP/1.1 200 OK\r\n\r\n");
  tc->loop(false);  // POST request is ready, but a delay is in effect
  assertTrue(pusher->getReadyToPost());
  delay(3000);
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  tc->loop(false);  // POST request is sent
  assertFalse(pusher->getReadyToPost());
  assertEqual(PROCESS_POST_RESPONSE, pusher->getState());
  assertFalse(pusher->getShouldSendHeadRequest());
  tc->loop(false);  // "HTTP/1.1 200 OK" is received
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  assertTrue(pusher->getShouldSendHeadRequest());
  assertFalse(pusher->getReadyToPost());
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, OAP_SERVER_PORT);

  // Send HEAD request to server and receive 200 OK
  EthernetClient::startMockServer(pusher->getServerDomain(), (uint32_t)0, OAP_SERVER_PORT, (const uint8_t*)"HTTP/1.1 200 OK\r\n"
                                  "Date: Mon, 21 Aug 2023 16:33:54 GMT\r\n"
                                  "Content-Length: 664\r\n"
                                  "Content-Type: text/html\r\n"
                                  "\r\n");
  tc->loop(false);  // too soon; HEAD request is not sent
  delay(3000);
  tc->loop(false);  // HEAD request is sent
  assertFalse(pusher->getShouldSendHeadRequest());
  assertEqual(PROCESS_HEAD_RESPONSE, pusher->getState());
  assertFalse(pusher->getReadyToPost());
  SD_TC::instance()->format();                      // wipe alert log file
  SD_TC::instance()->updateAlertFileSizeForTest();  // size to zero
  SD_TC::instance()->writeAlert("some data here");  // plus '\n' and the header row
  tc->loop(false);                                  // "200 OK" is received
  // assertFalse(pusher->getShouldSendHeadRequest());  // unless something else triggered a head request during one of
  // these loops
  assertFalse(pusher->getReadyToPost());  // because server has all bytes
  assertEqual(CLIENT_NOT_CONNECTED, pusher->getState());
  (pusher->getClient())->stop();
  EthernetClient::stopMockServer(pusher->getServerDomain(), (uint32_t)0, OAP_SERVER_PORT);
}

unittest_main()
