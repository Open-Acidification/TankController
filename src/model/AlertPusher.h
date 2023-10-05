#pragma once
#include <Arduino.h>

#include "wrappers/Ethernet_TC.h"

enum clientState_t { CLIENT_NOT_CONNECTED, PROCESS_HEAD_RESPONSE, PROCESS_POST_RESPONSE };

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
  const char *getServerDomain() {
    return serverDomain;
  }
  bool getShouldSendHeadRequest() {
    return shouldSendHeadRequest;
  }
  void setShouldSentHeadRequest(bool value) {
    shouldSendHeadRequest = value;
  }
  clientState_t getState() {
    return state;
  }
#endif

private:
  // class variables
  static AlertPusher *_instance;

  // instance variables
  EthernetClient client;
  clientState_t state = CLIENT_NOT_CONNECTED;
  uint32_t delayRequestsUntilTime = 40000;  // wait a bit before first request
  const char *serverDomain = "oap.cs.wallawalla.edu";
  char buffer[300];
  unsigned int index = 0;
  bool readyToPost = false;
  uint32_t serverFileSize = 0;
  bool shouldSendHeadRequest = false;

  // instance methods
  void loopHead();
  void loopPost();
  void sendPostRequest();
  void sendHeadRequest();
};
