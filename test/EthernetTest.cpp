#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "Ethernet_TC.h"

const int PIN = 4;

unittest(main) {
  GodmodeState* state = GODMODE();
  EEPROM_TC::instance()->setTankID(99);
  // Test singleton
  Ethernet_TC* singleton1 = nullptr;
  singleton1 = Ethernet_TC::instance();
  assertNotNull(singleton1);
  Ethernet_TC* singleton2 = nullptr;
  singleton2 = Ethernet_TC::instance();
  assertNotNull(singleton2);
  assertEqual(singleton1, singleton2);
  assertEqual(HIGH, state->digitalPin[PIN]);

  uint8_t* bytes = singleton1->getMac();
  assertEqual(0x90, (int)bytes[0]);
  assertEqual(0xA2, (int)bytes[1]);
  assertEqual(0xDA, (int)bytes[2]);
  assertEqual(0xFB, (int)bytes[3]);
  assertEqual(0xF6, (int)bytes[4]);
  assertEqual(0xF1, (int)bytes[5]);

  GODMODE()->resetClock();
  delay(1);
  singleton1->readMac(true);
  bytes = singleton1->getMac();
  assertEqual(0x90, (int)bytes[0]);
  assertEqual(0xA2, (int)bytes[1]);
  assertEqual(0xDA, (int)bytes[2]);
  assertEqual(0xFC, (int)bytes[3]);
  assertEqual(0xF7, (int)bytes[4]);
  assertEqual(0xF2, (int)bytes[5]);

  // Test that the default IP was used as a fall back (Assuming that there is no DHCP server to talk to)
  // test that the default is the defaultIP[3] plus the tank ID
  int tankID = EEPROM_TC::instance()->getTankID();
  IPAddress defaultIP(192, 168, 1, (10 + tankID));
  assertEqual(singleton1->getIP(), defaultIP);

  // Test DHCP is being maintained
  singleton1->loop();
  singleton1->loop();
  singleton1->loop();
  assertEqual(singleton1->getNumAttemptedDHCPReleases(), 3);
}

unittest(begin_with_DHCP) {
  Ethernet.mockDHCP(IPAddress(192, 168, 1, 42));
  assertTrue(Ethernet_TC::instance(true)->isConnectedToNetwork());
}

unittest(begin_without_DHCP) {
  Ethernet.mockDHCP(IPAddress((uint32_t)0));
  assertFalse(Ethernet_TC::instance(true)->isConnectedToNetwork());
}

unittest_main()
