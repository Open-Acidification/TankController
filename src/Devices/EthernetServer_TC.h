#pragma once

#include <Arduino.h>

#include "SD_TC.h"
#ifdef MOCK_PINS_COUNT
#include "Ethernet.h"
#else
#include <Ethernet.h>
#endif

enum serverState_t {
  NOT_CONNECTED,
  READ_HEADERS,
  GET_REQUEST,
  POST_REQUEST,
  IN_PROGRESS,
  IN_TRANSFER,
  FINISHED,
  ARBITRARY_PATH
};

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
  serverState_t getState() const {
    return state;
  }
  void arbitraryPath();
  void loop();
  void writeToClientBuffer(char*, bool);
  float charArrToFloat(char* input);

private:
  // class variables
  static EthernetServer_TC* _instance;

  // instance variables
  EthernetClient client;
  serverState_t state = NOT_CONNECTED;
  char buffer[512];
  char boundary[9];
  unsigned int bufferContentsSize = 0;
  unsigned long connectedAt = 0;
  File file;
  int startTime;

  // instance methods: constructor
  EthernetServer_TC(uint16_t port);
  // instance methods: utility
  void sendHeadersWithSize(uint32_t size);
  void sendFileHeadersWithSize(uint32_t size);
  void sendRedirectHeaders();
  void sendBadRequestHeaders();
  void sendBadBody();
  int weekday(int year, int month, int day);
  // instance methods: HTTP
  void get();
  void post();
  void echo();
  void apiHandler();
  void current();
  void display();
  void keypress();
  void rootdir();
  void testReadSpeed();
  void testWriteSpeed();
  bool isFileRequest();
  void fileSetup();
  bool fileContinue();
};
