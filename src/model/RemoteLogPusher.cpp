#include "model/RemoteLogPusher.h"

#include "Version.h"
#include "model/PHControl.h"
#include "model/TC_util.h"
#include "wrappers/Ethernet_TC.h"
#include "wrappers/SD_TC.h"
#include "wrappers/Serial_TC.h"

const uint16_t PORT = 8080;

//  class variables
RemoteLogPusher* RemoteLogPusher::_instance = nullptr;

//  class methods
/**
 * @brief accessor for singleton
 *
 * @return RemoteLogPusher*
 */

RemoteLogPusher* RemoteLogPusher::instance() {
  if (!_instance) {
    _instance = new RemoteLogPusher();
  }
  return _instance;
}

//  instance methods
RemoteLogPusher::RemoteLogPusher() {
  this->buffer[0] = '\0';
}

bool RemoteLogPusher::isReadyToPost() {
  return _isReadyToPost && millis() > delayRequestsUntilTime && !(PHControl::instance()->isOn());
}

void RemoteLogPusher::loop() {
  if (!(Ethernet_TC::instance()->isConnectedToNetwork())) {
    return;
  }
  switch (state) {
    case CLIENT_NOT_CONNECTED:
      if (isReadyToPost()) {
        _isReadyToPost = false;
        sendPostRequest();
      } else if (shouldSendHeadRequest()) {
        _shouldSendHeadRequest = false;
        sendHeadRequest();
      } else {
        // nothing to do
      }
      break;
    case PROCESS_HEAD_RESPONSE:
      loopHead();
      break;
    case PROCESS_POST_RESPONSE:
      loopPost();
      break;
    default:
      break;
  }
}

void RemoteLogPusher::loopHead() {
  if (client.connected()) {
    if (client.available()) {  // bytes are remaining in the current packet
      int next;
      serial(F("RemoteLogPusher: received"));
      while ((next = client.read()) != -1) {  // Flawfinder: ignore
        if (next) {
          if (next == '\r') {
            buffer[index] = '\0';
            serial(F("  %s"), buffer);
            bool isDone = false;
            if (index >= 22 && memcmp_P(buffer, F("http/1.1 404 not found"), 22) == 0) {
              // File has not yet been created on server
              serverFileSize = (uint32_t)0;
              _isReadyToPost = true;
              isDone = true;
            } else if (index > 16 && memcmp_P(buffer, F("content-length: "), 16) == 0) {
              serverFileSize = strtoul(buffer + 16, nullptr, 10);
              uint32_t localFileSize = SD_TC::instance()->getRemoteFileSize();
              serial(F("RemoteLogPusher: local %lu bytes, cloud %lu bytes"), (uint32_t)localFileSize,
                     (uint32_t)serverFileSize);
              _isReadyToPost = serverFileSize < localFileSize;
              isDone = true;
            }
            if (isDone) {
              buffer[0] = '\0';
              index = 0;
              client.stop();
              state = CLIENT_NOT_CONNECTED;
              delayRequestsUntilTime = millis() + 3000;
              return;
            }
          } else if (next == '\n' || index == sizeof(buffer)) {
            serial(F("Buffer loop after filled with \"%s\""), buffer);
            buffer[0] = '\0';
            index = 0;
          } else {
            buffer[index++] = tolower(next);
          }
        }
      }
    }
  } else {
    state = CLIENT_NOT_CONNECTED;
    client.stop();
    delayRequestsUntilTime = millis() + 3000;
  }
}

void RemoteLogPusher::loopPost() {
  if (client.connected()) {
    if (client.available()) {  // bytes are remaining in the current packet
      int next;
      serial(F("RemoteLogPusher: received"));
      while ((next = client.read()) != -1) {  // Flawfinder: ignore
        if (next) {
          if (next == '\r') {
            buffer[index] = '\0';
            serial(F("  %s"), buffer);
            if (index >= 15 && memcmp_P(buffer, F("http/1.1 200 ok"), 15) == 0) {
              buffer[0] = '\0';
              index = 0;
              state = CLIENT_NOT_CONNECTED;
              client.stop();
              _shouldSendHeadRequest = true;
              delayRequestsUntilTime = millis() + 3000;
              return;
            }
          } else if (next == '\n' || index == sizeof(buffer)) {
            buffer[0] = '\0';
            index = 0;
          } else {
            buffer[index++] = tolower(next);
          }
        }
      }
    }
  } else {
    state = CLIENT_NOT_CONNECTED;
    client.stop();
    _shouldSendHeadRequest = true;
    delayRequestsUntilTime = millis() + 3000;
  }
}

/**
 * @brief attempt to push at the next opportunity
 *
 * This method is called when the remote log file is written to, and when the
 * bubbler is turned off.
 */
void RemoteLogPusher::pushSoon() {
  _shouldSendHeadRequest = true;
}

/**
 * @brief send request to server for size of remote log file
 *
 */
void RemoteLogPusher::sendHeadRequest() {
  serial(F("RemoteLogPusher: attempting HEAD request"));
  static const char format[] PROGMEM =
      "HEAD /logs/%s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "User-Agent: TankController/%s\r\n"
      "Accept: text/plain\r\n"
      "Connection: Close\r\n"
      "\r\n";
  snprintf_P(buffer, sizeof(buffer), (PGM_P)format, SD_TC::instance()->getRemoteLogName(), serverDomain, VERSION);
  if (client.connected() || client.connect(serverDomain, PORT) == 1) {  // this is a blocking step
    serial(F("RemoteLogPusher: connected to %s, sending..."), serverDomain);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
  } else {
    serial(F("RemoteLogPusher: connection to %s failed"), serverDomain);
    // "_shouldSendHeadRequest = true;" would retry next loop but we'll try within one minute anyway
  }
  buffer[0] = '\0';
  state = PROCESS_HEAD_RESPONSE;
}

void RemoteLogPusher::sendPostRequest() {
  serial(F("RemoteLogPusher: attempting POST request"));
  char data[300];
  SD_TC::instance()->getRemoteLogContents(data, sizeof(data), serverFileSize);
  static const char format[] PROGMEM =
      "POST /logs/%s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Content-Type: text/plain\r\n"
      "User-Agent: TankController/%s\r\n"
      "Content-Length: %i\r\n"
      "Connection: Close\r\n"
      "\r\n";
  snprintf_P(buffer, sizeof(buffer), (PGM_P)format, SD_TC::instance()->getRemoteLogName(), serverDomain, VERSION,
             strnlen(data, sizeof(data)));
  if (client.connected() || client.connect(serverDomain, PORT) == 1) {  // this is a blocking step
    serial(F("RemoteLogPusher: connected to %s, sending..."), serverDomain);
    serial(data);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
    client.write(data, strnlen(data, sizeof(data)));
  } else {
    serial(F("RemoteLogPusher: connection to %s failed"), serverDomain);
  }
  buffer[0] = '\0';
  state = PROCESS_POST_RESPONSE;
}

bool RemoteLogPusher::shouldSendHeadRequest() {
  return _shouldSendHeadRequest && millis() > delayRequestsUntilTime && !(PHControl::instance()->isOn());
}
