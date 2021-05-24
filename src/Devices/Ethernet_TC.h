/**
 * Based off Arduino's Ethernet library
 *Implements a wrapper for the Ethernet Class
 */

#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <Ethernet_CI.h>
#else
#include <Ethernet.h>
#endif

class Ethernet_TC {
public:
  static Ethernet_TC *instance();
  IPAddress getIP() {
    return IP;
  };
  byte *getMac() {
    return mac;
  }
  int getNumAttemptedDHCPReleases() {
    return numAttemptedDHCPReleases;
  };
  void renewDHCPLease();
  bool gotDHCPLease() {
    return IP != defaultIP;
  };

protected:
  Ethernet_TC();

private:
  static Ethernet_TC *_instance;
  byte mac[6] = {0x90, 0xA2, 0xDA, 0x00, 0x00, 0x00};
  IPAddress defaultIP = IPAddress(192, 168, 1, 2);
  IPAddress time_serverIP;
  const int IO_PIN = 4;
  IPAddress IP;
  unsigned long previous_lease = 0;
  const unsigned long LEASE_INTERVAL = 345600000;  // 4 days in milliseconds
  // testing
  int numAttemptedDHCPReleases = 0;
};
