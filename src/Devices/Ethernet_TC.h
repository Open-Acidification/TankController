/**
 * Based off Arduino's Ethernet library
 *Implements a wrapper for the Ethernet Class
 */

#pragma once

#include <Arduino.h>
#include <Ethernet_CI.h>

class Ethernet_TC {
public:
  static Ethernet_TC *instance();
  IPAddress getIP() {
    return IP;
  };
  byte *getMac() {
    return mac;
  }
  uint16_t getNumAttemptedDHCPReleases() {
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
  IPAddress defaultIP = IPAddress(192, 168, 1, 10);
  IPAddress time_serverIP;
  const int IO_PIN = 4;
  IPAddress IP;
  uint32_t previous_lease = 0;
  const uint32_t LEASE_INTERVAL = 345600000;  // 4 days in milliseconds

  // testing
  uint16_t numAttemptedDHCPReleases = 0;
};
