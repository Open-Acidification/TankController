#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <Ethernet.h>
#else
#include <Ethernet.h>
#endif

enum serverState_t { NOT_CONNECTED, READ_REQUEST, GET_REQUEST, HAS_REQUEST, HAS_HEADERS };

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
  const char* className() const {
    return "EthernetServer_TC";
  }
  void loop();

private:
  // class variables
  static EthernetServer_TC* _instance;

  // instance variables
  EthernetClient client;
  serverState_t state = NOT_CONNECTED;
  char buffer[128];
  int bufferContentsSize = 0;
  unsigned long connectedAt = 0;

  // instance methods: constructor
  EthernetServer_TC(uint16_t port);
  // instance methods: utility
  void sendHeadersWithSize(uint32_t size);
  int weekday(int year, int month, int day);
  // instance methods: HTTP
  void echo();
  bool file();
  void get();
};
