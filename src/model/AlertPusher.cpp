#include "model/AlertPusher.h"

#include "Version.h"
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
  if (client.connected()) {
    delayRequestsUntilTime = millis();  // now connected; no need to delay after this loop
    if (client.available()) {           // bytes are remaining in the current packet
      int next;
      while ((next = client.read()) != -1) {  // Flawfinder: ignore
        if (next) {
          if (next == '\r') {
            buffer[index] = '\0';
            switch (state) {
              case HEAD_REQUEST:
                if (index > 16 && memcmp_P(F("content-length: "), buffer, 16) == 0) {
                  serverFileSize = atoi(buffer + 16);
                  if (serverFileSize < SD_TC::instance()->getAlertFileSize()) {
                    readyToPost = true;
                  }
                } else if (index > 13 && memcmp_P(F("404 not found"), buffer, 13) == 0) {
                  // File has not yet been created on server
                  serverFileSize = 0;
                  readyToPost = true;
                }
                break;
              case POST_REQUEST:
                if (index > 6 && memcmp_P(F("200 ok"), buffer, 6) == 0) {
                  shouldSendHeadRequest = true;  // determine whether more should be sent
                }
                break;
              default:
                break;
            }
          } else if (next == '\n' || index == sizeof(buffer)) {
            index = 0;
            buffer[index] = '\0';
          } else {
            buffer[index++] = tolower(next);
          }
        }
      }
    }
  } else {
    // will this close every 15 ms?
    client.stop();
    unsigned long now = millis();
    if (readyToPost && now >= delayRequestsUntilTime) {  // TODO: and bubbler is off?
      delayRequestsUntilTime = now + 5000;
      sendPostRequest();  // can block for 1 sec?
      readyToPost = false;
    } else if (shouldSendHeadRequest && now >= delayRequestsUntilTime) {  // TODO: and bubbler is off?
      delayRequestsUntilTime = now + 5000;
      sendHeadRequest();  // can block for 1 sec?
      shouldSendHeadRequest = false;
    }
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
 * @brief ask for size of file in the cloud
 *
 */
void AlertPusher::sendHeadRequest() {
  serial(F("AlertPusher: sending HEAD request"));
  state = HEAD_REQUEST;
  static const char format[] PROGMEM =
      "HEAD /logs/%s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "User-Agent: TankController/%s\r\n"
      "Accept: text/plain\r\n"
      "Connection: Keep-Alive\r\n"
      "\r\n";
  snprintf_P(buffer, sizeof(buffer), (PGM_P)format, SD_TC::instance()->getAlertFileName(), serverDomain, VERSION);
  if (client.connect(serverDomain, 80) == 1) {
    serial(F("connected to %s"), serverDomain);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
  } else {
    serial(F("connection to %s failed"), serverDomain);
    shouldSendHeadRequest = true;
  }
  buffer[0] = '\0';
}

void AlertPusher::sendPostRequest() {
  serial(F("AlertPusher: sending POST request"));
  state = POST_REQUEST;
  char nextAlertString[300];
  SD_TC::instance()->getAlert(nextAlertString, sizeof(nextAlertString), serverFileSize);
  static const char format[] PROGMEM =
      "POST /logs/%s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Content-Type: text/plain\r\n"
      "User-Agent: TankController/%s\r\n"
      "Content-Length: %i\r\n"
      "Connection: close\r\n"
      "\r\n";
  snprintf_P(buffer, sizeof(buffer), (PGM_P)format, SD_TC::instance()->getAlertFileName(), serverDomain, VERSION,
             strnlen(nextAlertString, sizeof(nextAlertString)));
  // TODO: finish method
  if (client.connect(serverDomain, 80) == 1) {
    serial(F("connected to %s"), serverDomain);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
    client.write(nextAlertString, strnlen(nextAlertString, sizeof(nextAlertString)));
    client.write('\r');
    client.write('\n');
  } else {
    serial(F("connection to %s failed"), serverDomain);
    shouldSendHeadRequest = true;
  }
}
