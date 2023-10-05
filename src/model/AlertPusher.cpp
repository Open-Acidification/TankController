#include "model/AlertPusher.h"

#include "Version.h"
#include "model/TC_util.h"
#include "wrappers/Ethernet_TC.h"
#include "wrappers/SD_TC.h"
#include "wrappers/Serial_TC.h"

//  class variables
AlertPusher* AlertPusher::_instance = nullptr;

//  class methods
/**
 * @brief accessor for singleton
 *
 * @return AlertPusher*
 */
AlertPusher* AlertPusher::instance() {
  if (!_instance) {
    _instance = new AlertPusher();
  }
  return _instance;
}

//  instance methods
AlertPusher::AlertPusher() {
  this->buffer[0] = '\0';
}

void AlertPusher::loop() {
  if (!(Ethernet_TC::instance()->isConnectedToNetwork())) {
    return;
  }
  switch (state) {
    case CLIENT_NOT_CONNECTED:
      if (readyToPost && millis() > delayRequestsUntilTime) {  // TODO: and bubbler is off?
        readyToPost = false;
        sendPostRequest();
      } else if (shouldSendHeadRequest && millis() > delayRequestsUntilTime) {  // TODO: and bubbler is off?
        shouldSendHeadRequest = false;
        sendHeadRequest();
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

void AlertPusher::loopHead() {
  if (client.connected()) {
    if (client.available()) {  // bytes are remaining in the current packet
      int next;
      serial(F("AlertPusher: received"));
      while ((next = client.read()) != -1) {  // Flawfinder: ignore
        if (next) {
          if (next == '\r') {
            buffer[index] = '\0';
            serial(F("  %s"), buffer);
            if (index >= 22 && memcmp_P(buffer, F("http/1.1 404 not found"), 22) == 0) {
              // File has not yet been created on server
              serverFileSize = (uint32_t)0;
              buffer[0] = '\0';
              index = 0;
              state = CLIENT_NOT_CONNECTED;
              client.stop();
              readyToPost = true;
              delayRequestsUntilTime = millis() + 3000;
              return;
            } else if (index > 16 && memcmp_P(buffer, F("content-length: "), 16) == 0) {
              serverFileSize = strtoul(buffer + 16, nullptr, 10);
              uint32_t localFileSize = SD_TC::instance()->getAlertFileSize();
              buffer[0] = '\0';
              index = 0;
              serial(F("AlertPusher: local %lu bytes, cloud %lu bytes"), (uint32_t)localFileSize,
                     (uint32_t)serverFileSize);
              if (serverFileSize < localFileSize) {
                state = CLIENT_NOT_CONNECTED;
                client.stop();
                readyToPost = true;
                delayRequestsUntilTime = millis() + 3000;
              } else {
                state = CLIENT_NOT_CONNECTED;
                client.stop();
                delayRequestsUntilTime = millis() + 3000;
              }
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
    delayRequestsUntilTime = millis() + 3000;
  }
}

void AlertPusher::loopPost() {
  if (client.connected()) {
    if (client.available()) {  // bytes are remaining in the current packet
      int next;
      serial(F("AlertPusher: received"));
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
              shouldSendHeadRequest = true;
              delayRequestsUntilTime = millis() + 3000;
              return;
            }
          }
        } else if (next == '\n' || index == sizeof(buffer)) {
          buffer[0] = '\0';
          index = 0;
        } else {
          buffer[index++] = tolower(next);
        }
      }
    }
  } else {
    state = CLIENT_NOT_CONNECTED;
    client.stop();
    shouldSendHeadRequest = true;
    delayRequestsUntilTime = millis() + 3000;
  }
}

/**
 * @brief attempt to push at the next opportunity
 *
 */
void AlertPusher::pushSoon() {
  shouldSendHeadRequest = true;
}

/**
 * @brief send request to server for size of alert file
 *
 */
void AlertPusher::sendHeadRequest() {
  serial(F("AlertPusher: attempting HEAD request"));
  state = PROCESS_HEAD_RESPONSE;
  static const char format[] PROGMEM =
      "HEAD /logs/%s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "User-Agent: TankController/%s\r\n"
      "Accept: text/plain\r\n"
      "Connection: Close\r\n"
      "\r\n";
  snprintf_P(buffer, sizeof(buffer), (PGM_P)format, SD_TC::instance()->getAlertFileName(), serverDomain, VERSION);
  if (client.connected() || client.connect(serverDomain, PORT) == 1) {  // this is a blocking step
    serial(F("AlertPusher: connected to %s, sending..."), serverDomain);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
  } else {
    serial(F("AlertPusher: connection to %s failed"), serverDomain);
    // "shouldSendHeadRequest = true;" would retry next loop but we'll try within one minute anyway
  }
  buffer[0] = '\0';
}

void AlertPusher::sendPostRequest() {
  serial(F("AlertPusher: attempting POST request"));
  state = PROCESS_POST_RESPONSE;
  char data[300];
  SD_TC::instance()->getAlert(data, sizeof(data), serverFileSize);
  static const char format[] PROGMEM =
      "POST /logs/%s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Content-Type: text/plain\r\n"
      "User-Agent: TankController/%s\r\n"
      "Content-Length: %i\r\n"
      "Connection: Close\r\n"
      "\r\n";
  snprintf_P(buffer, sizeof(buffer), (PGM_P)format, SD_TC::instance()->getAlertFileName(), serverDomain, VERSION,
             strnlen(data, sizeof(data)));
  if (client.connected() || client.connect(serverDomain, PORT) == 1) {  // this is a blocking step
    serial(F("AlertPusher: connected to %s, sending..."), serverDomain);
    serial(data);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
    client.write(data, strnlen(data, sizeof(data)));
  } else {
    serial(F("AlertPusher: connection to %s failed"), serverDomain);
  }
  buffer[0] = '\0';
}
