#pragma once
#include <Arduino.h>

#include "wrappers/Ethernet_TC.h"

enum clientState_t { CLIENT_HEAD_REQUEST, CLIENT_POST_REQUEST };

class AlertPusher {
public:
  // class methods
  static AlertPusher *instance();

  // instance methods
  AlertPusher();
  void loop();
  void pushSoon();

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  EthernetClient *getClient() {
    return &client;
  }
  bool getReadyToPost() {
    return readyToPost;
  }
  const char *getServerDomain() {
    return serverDomain;
  }
  bool getShouldSendHeadRequest() {
    return shouldSendHeadRequest;
  }
#endif

private:
  // class variables
  static AlertPusher *_instance;

  // instance variables
  EthernetClient client;
  clientState_t state;
  uint32_t delayRequestsUntilTime = 40000;  // wait a bit before first request
  const char *serverDomain = "oap.cs.wallawalla.edu";
  char buffer[200];
  uint8_t index = 0;
  uint64_t serverFileSize = 0;
  bool readyToPost = false;
  bool shouldSendHeadRequest = false;

  // instance methods
  void sendPostRequest();
  void sendHeadRequest();
};
