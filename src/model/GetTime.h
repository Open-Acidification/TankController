#pragma once
#include <Arduino.h>

#include "wrappers/Ethernet_TC.h"

class GetTime {
public:
  // class methods
  static GetTime *instance(int tzOffsetHrs = -7);

  // instance methods
  GetTime(int tzOffsetHrs = -7);
  EthernetClient *getClient() {
    return &client;
  }
  const char *getServerDomain() {
    return serverDomain;
  }
  void loop();

private:
  // class variables
  static GetTime *_instance;

  // instance variables
  EthernetClient client;
  int tzOffsetHrs;
  uint32_t nextSendTime = 30000;                      // wait a bit for first reading
  const uint32_t interval = 24l * 60l * 60l * 1000l;  // once per day
  const char *serverDomain = "oap.cs.wallawalla.edu";
  char buffer[100];
  uint8_t index = 0;

  // instance method
  void sendRequest();
};
