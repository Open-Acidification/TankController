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
  void readMac(bool forceReset = false);
  uint16_t getNumAttemptedDHCPReleases() {
    return numAttemptedDHCPReleases;
  };
  void loop();

protected:
  Ethernet_TC();

private:
  static Ethernet_TC *_instance;
  byte mac[6] = {0x90, 0xA2, 0xDA, 0x00, 0x00, 0x00};
  IPAddress defaultIP = IPAddress(192, 168, 1, 10);
  IPAddress time_serverIP;
  const int SD_SELECT_PIN = 4;
  IPAddress IP;

  // testing
  uint16_t numAttemptedDHCPReleases = 0;
};
