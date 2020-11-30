#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <Ethernet_CI.h>
#else
#include <Ethernet.h>
#endif

/**
 * EthernetServer_TC provides wrapper for web server for TankController
 *
 * If running in test mode, superclass = EthernetServer_CI
 */
class EthernetServer_TC : public EthernetServer {
public:
  // class methods
  static EthernetServer_TC* instance();
  // instance methods
  void handleRequest();
  virtual String className() const {
    return "EthernetServer_TC";
  }

private:
  // class variables
  static EthernetServer_TC* _instance;

  // instance methods
  EthernetServer_TC(uint16_t port);
};
