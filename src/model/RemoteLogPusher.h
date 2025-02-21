#pragma once
#include <Arduino.h>

#include "wrappers/Ethernet_TC.h"

#define MAX_FILE_NAME_LENGTH 28

/*
 * @brief RemoteLogPusher is a singleton that sends data records and remote logs to the server.
 *
 * The server is expected to be running extras/log_file_server.
 * The ideas is that while the device records data to the SD card's 1-second log and
 * 1-minute log, it also sends the 1-minute log records to the server with configuration
 * changes and certain warning and error conditions.
 *
 * Eventually, the server will send remote logs to a user's phone.
 *
 * The device tries to keep the server up-to-date on the latest data by comparing the
 * file size reported by the server (in a HEAD request) with the file size on the SD card.
 * If the server has a smaller file, the device sends data from the indicated point to the
 * end of the file.
 *
 * The instance is called each time through the TankController's loop() and operates
 * with a state machine.
 *
 * If the state is CLIENT_NOT_CONNECTED and there is new data (indicated by the
 * _shouldSendHeadRequest flag), it sends a HEAD request and changes the state to
 * PROCESS_HEAD_RESPONSE.
 *
 * If the state is PROCESS_HEAD_RESPONSE, it reads the response from the server and if it
 * is a 404, it transitions to the CLIENT_NOT_CONNECTED state and sets a _isReadyToPost flag.
 * If the response is a 200, it transitions to the CLIENT_NOT_CONNECTED state and sets a
 * _isReadyToPost flag if the reported file size is less than the file size on the SD card.
 *
 * If the state is CLIENT_NOT_CONNECTED and there is a _isReadyToPost flag, reads from the
 * SD card and sends a POST request and changes the state to PROCESS_POST_RESPONSE.
 *
 * If the state is PROCESS_POST_RESPONSE, it reads the response from the server and if it
 * a 200, it transitions to the CLIENT_NOT_CONNECTED state and sets a _shouldSendHeadRequest
 * flag to see if there is more that should be sent.
 *
 */

enum clientState_t { CLIENT_NOT_CONNECTED, PROCESS_HEAD_RESPONSE, PROCESS_POST_RESPONSE };

class RemoteLogPusher {
public:
  // class methods
  static RemoteLogPusher *instance();
  static void deleteInstance();

  // instance methods
  RemoteLogPusher();
  const char* getRemoteLogName() {
    return remoteLogName;
  }
  bool isReadyToPost();
  void loop();
  void pushSoon();
  void setRemoteLogName(const char* newFileName = nullptr);
  bool shouldSendHeadRequest();

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  EthernetClient *getClient() {
    return &client;
  }
  const char *getServerDomain() {
    return serverDomain;
  }
  bool basicShouldSendHeadRequest() {
    return _shouldSendHeadRequest;
  }

  void setShouldSentHeadRequest(bool value) {
    _shouldSendHeadRequest = value;
  }
  clientState_t getState() {
    return state;
  }
  void reset() {
    _instance = nullptr;
  }
#endif

private:
  // class variables
  static RemoteLogPusher *_instance;

  // instance variables
  EthernetClient client;
  clientState_t state = CLIENT_NOT_CONNECTED;
  uint32_t delayRequestsUntilTime = 40000;  // wait a bit before first request
  const char *serverDomain = "oap.cs.wallawalla.edu";
  char remoteLogName[MAX_FILE_NAME_LENGTH + 5] = "";  // add ".log" with null-terminator
  char buffer[300];
  unsigned int index = 0;
  bool _isReadyToPost = false;
  uint32_t serverFileSize = 0;
  bool _shouldSendHeadRequest = false;

  // instance methods
  void loopHead();
  void loopPost();
  void sendPostRequest();
  void sendHeadRequest();
};
