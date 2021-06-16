#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/EEPROM_TC.h"
#include "Ethernet_TC.h"

const int PIN = 4;

unittest(Main) {
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
  int tankID = EEPROM_TC::instance()->getTankID();

  // Test that the default IP was used as a fall back (Assuming that there is no DHCP server to talk to)
  // test that the default is the defaultIP[3] plus the tank ID
  IPAddress defaultIP(192, 168, 1, (10 + tankID));
  assertEqual(singleton1->getIP(), defaultIP);

  // Test DHCP is being maintained
  singleton1->renewDHCPLease();
  singleton1->renewDHCPLease();
  singleton1->renewDHCPLease();
  assertEqual(singleton1->getNumAttemptedDHCPReleases(), 3);
}

unittest_main()
