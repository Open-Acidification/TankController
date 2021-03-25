/**
 * Based off Arduino's Ethernet library
 *Implements a wrapper for the Ethernet Class
 */

#pragma once

#include "Ethernet.h"

class Ethernet_TC {
public:
  static Ethernet_TC *getInstance();
  IPAddress getIP() {
    return IP;
  };
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
  IPAddress IP;
  unsigned long previous_lease = 0;
  const unsigned long LEASE_INTERVAL = 345600000;  // 4 days in milliseconds

  // testing
  int numAttemptedDHCPReleases = 0;
};
