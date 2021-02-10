#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Ethernet_TC.h"

unittest(Main) {
  // Test singleton
  Ethernet_TC* singleton1 = nullptr;
  singleton1 = Ethernet_TC::getInstance();
  assertNotNull(singleton1);
  Ethernet_TC* singleton2 = nullptr;
  singleton2 = Ethernet_TC::getInstance();
  assertNotNull(singleton2);
  assertEqual(singleton1, singleton2);

  // Test that the default Ip was used as a fall back (Assuming that there is no DHCP server to talk to)
  IPAddress defaultIP(192, 168, 1, 2);
  assertTrue(singleton1->getIP() == defaultIP);

  // Test DHCP is being maintained
  singleton1->renewDHCPLease();
  singleton1->renewDHCPLease();
  singleton1->renewDHCPLease();
  assertEqual(singleton1->getNumAttemptedDHCPReleases(), 3);
}

unittest_main()
