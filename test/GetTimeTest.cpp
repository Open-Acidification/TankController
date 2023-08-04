#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/EthernetServer_TC.h"
#include "Devices/GetTime.h"
#include "Devices/PHControl.h"
#include "Devices/ThermalProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "TankController.h"

EthernetClient *pClient;
GodmodeState *state = GODMODE();
GetTime *pGetTime;
TankController *tc;
ThermalProbe_TC *tempProbe;
PHControl *controlSolenoid;

unittest_setup() {
  state->reset();
  tc = TankController::instance();
  EEPROM_TC::instance()->setTankID(99);
  pGetTime = GetTime::instance();
  pClient = pGetTime->getClient();
  DateTime_TC now(2021, 6, 8, 15, 25, 15);
  now.setAsCurrent();
  state->serialPort[0].dataOut = "";  // clear serial output
}

unittest_teardown() {
  if (pClient->writeBuffer()) {
    pClient->writeBuffer()->clear();
  }
  pClient->stop();  // clears the readBuffer (but not the write buffer!?)
  EthernetClient::stopMockServer(pGetTime->getServerDomain(), (uint32_t)0, 80);
}

unittest(without_DHCP) {
  Ethernet.mockDHCP(IPAddress((uint32_t)0));
  assertFalse(Ethernet_TC::instance(true)->getIsUsingDHCP());
  EthernetClient::startMockServer(pGetTime->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());
  delay(45 * 1000);  // wait for 45 seconds to ensure we do not send
  tc->loop(false);
  assertFalse(pClient->connected());
}

unittest(with_DHCP) {
  Ethernet.mockDHCP(IPAddress(192, 168, 1, 42));
  assertTrue(Ethernet_TC::instance(true)->getIsUsingDHCP());
  EthernetClient::startMockServer(pGetTime->getServerDomain(), (uint32_t)0, 80,
                                  (const uint8_t *)"HTTP/1.1 404 Not Found\r\n"
                                                   "Date: Wed, 19 Jul 2023 04:18:52 GMT\r\n"
                                                   "Content-Type: text/html\r\n"
                                                   "\r\n");
  assertFalse(pClient->connected());  // not yet connected!
  delay(15 * 1000);                   // Allow bubbler to be turned off
  tc->loop(false);
  delay(30 * 1000);  // Wait for time query
  assertEqual("2021-06-08 15:26", DateTime_TC::now().as16CharacterString());
  tc->loop(false);
  assertEqual("2023-07-18 21:18", DateTime_TC::now().as16CharacterString());
  assertTrue(pClient->connected());
  pClient->stop();  // clears the readBuffer (but not the write buffer!?)
  EthernetClient::stopMockServer(pGetTime->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());

  EthernetClient::startMockServer(pGetTime->getServerDomain(), (uint32_t)0, 80,
                                  (const uint8_t *)"HTTP/1.1 404 Not Found\r\n"
                                                   "Date: Wed, 20 Jul 2023 14:18:52 GMT\r\n"
                                                   "Content-Type: text/html\r\n"
                                                   "\r\n");
  assertFalse(pClient->connected());  // not yet connected!
  delay(23 * 60 * 60 * 1000);         // should not be any change
  tc->loop(false);
  assertEqual("2023-07-19 20:18", DateTime_TC::now().as16CharacterString());
  delay(1 * 60 * 60 * 1000);  // now should change
  tc->loop(false);
  assertEqual("2023-07-20 07:18", DateTime_TC::now().as16CharacterString());
  assertTrue(pClient->connected());
  pClient->stop();  // clears the readBuffer (but not the write buffer!?)
  EthernetClient::stopMockServer(pGetTime->getServerDomain(), (uint32_t)0, 80);
  assertFalse(pClient->connected());
}

unittest_main()
