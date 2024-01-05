/**
 * Based off Arduino's Ethernet library
 *Implements a wrapper for the Ethernet Class
 */

#pragma once

#include <Arduino.h>
#include <Ethernet.h>

#define OAP_SERVER_PORT 8080

class Ethernet_TC {
public:
  static Ethernet_TC *instance(bool reset = false);
  IPAddress getIP() {
    return IP;
  };
  bool isConnectedToNetwork() {
    return isUsingDHCP;
  }
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
  bool isUsingDHCP = false;

  // testing
  uint16_t numAttemptedDHCPReleases = 0;
};
