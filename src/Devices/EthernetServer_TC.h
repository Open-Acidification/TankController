#pragma once

#include <Arduino.h>

#include "SD_TC.h"
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
#include "Ethernet.h"
#else
#include <Ethernet.h>
#endif

#define TIMEOUT 5000
#define HTTP_REDIRECT 303
#define HTTP_BAD_REQUEST 400
#define HTTP_NOT_FOUND 404
#define HTTP_NOT_PERMITTED 405
#define HTTP_TIMEOUT 408
#define HTTP_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501

enum serverState_t {
  NOT_CONNECTED,
  READ_REQUEST,
  GET_REQUEST,
  POST_REQUEST,
  PUT_REQUEST,
  OPTIONS_REQUEST,
  COUNTING_FILES,
  LISTING_FILES,
  IN_TRANSFER,
  FINISHED
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
  void loop();
  void writeToClientBuffer(const char*, bool);
  void sendHeadersForRootdir(int);

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
  void sendResponse(int);
  void sendCurrentRedirect();
  void sendDisplayRedirect();
  int weekday(int year, int month, int day);
  // instance methods: HTTP
  void get();
  void post();
  void put();
  void options();
  void echo();
  void apiHandler();
  void currentData();
  void display();
  void keypress();
  void rootdirSetup();
  void rootdir();
  void testReadSpeed();
  void testWriteSpeed();
  bool isRequestForExistingFile();
  void fileSetup();
  bool fileContinue();
};
