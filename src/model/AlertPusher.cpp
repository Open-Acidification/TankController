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
  if (client.connected()) {
    delayRequestsUntilTime = millis();  // now connected; no need to delay after this loop
    if (client.available()) {           // bytes are remaining in the current packet
      int next;
      serial(F("AlertPusher received the following (index %i):"), index);
      while ((next = client.read()) != -1) {  // Flawfinder: ignore
        if (next) {
          if (next == '\r') {
            buffer[index] = '\0';
            serial(F("  %s"), buffer);
            switch (state) {
              case CLIENT_HEAD_REQUEST:
                // serial(F("AlertPusher: Processing response to HEAD; index %i"), index);
                shouldSendHeadRequest = false;
                if (index > 16 && memcmp_P(buffer, F("content-length: "), 16) == 0) {
                  // serial(F("AlertPusher: Found content-length %i"), atoi(buffer + 16));
                  serial(F("  ..."));
                  serverFileSize = atoi(buffer + 16);
                  serial(F("  %i bytes"), serverFileSize);
                  uint64_t localFileSize = SD_TC::instance()->getAlertFileSize();
                  serial(F("  %i bytes"), serverFileSize);
                  serial(F("%i,%i,%i,%i,%i"), 1, 2, 3, localFileSize, serverFileSize);
                  serial(F("AlertPusher: local %i bytes, cloud %i bytes"), localFileSize, serverFileSize);
                  if (serverFileSize + 1 < localFileSize) {
                    readyToPost = true;
                    serial(F("AlertPusher: Attempting to POST"));
                    // serial(F("AlertPusher: Ready to append to file via POST"));
                  } else if (!readyToPost) {  // already got 404?
                    serial(F("AlertPusher: Cloud file is up-to-date"));
                  }
                  index = 0;
                  client.stop();
                  return;
                } else if (index >= 22 && memcmp_P(buffer, F("http/1.1 404 not found"), 22) == 0) {
                  // File has not yet been created on server
                  // serial(F("AlertPusher: Found 404 response"));
                  serial(F("  ..."));
                  serverFileSize = 0;
                  readyToPost = true;
                  buffer[0] = '\0';
                  serial(F("AlertPusher: Ready to create new file via POST"));
                  index = 0;
                  client.stop();
                  return;
                }
                break;
              case CLIENT_POST_REQUEST:
                // serial(F("AlertPusher: Processing response to POST"));
                readyToPost = false;
                if (index >= 15 && memcmp_P(buffer, F("http/1.1 200 ok"), 6) == 0) {
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
      sendPostRequest();                                                  // can block for 1 sec?
    } else if (shouldSendHeadRequest && now >= delayRequestsUntilTime) {  // TODO: and bubbler is off?
      delayRequestsUntilTime = now + 5000;
      sendHeadRequest();  // can block for 1 sec?
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
  state = CLIENT_HEAD_REQUEST;
  static const char format[] PROGMEM =
      "HEAD /logs/%s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "User-Agent: TankController/%s\r\n"
      "Accept: text/plain\r\n"
      "Connection: Keep-Alive\r\n"
      "\r\n";
  snprintf_P(buffer, sizeof(buffer), (PGM_P)format, SD_TC::instance()->getAlertFileName(), serverDomain, VERSION);
  if (client.connect(serverDomain, PORT) == 1) {
    serial(F("AlertPusher: HEAD connected to %s"), serverDomain);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
  } else {
    serial(F("AlertPusher: HEAD connection to %s failed"), serverDomain);
    shouldSendHeadRequest = true;
  }
  buffer[0] = '\0';
}

void AlertPusher::sendPostRequest() {
  serial(F("AlertPusher: sending POST request"));
  state = CLIENT_POST_REQUEST;
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
  if (client.connect(serverDomain, PORT) == 1) {
    serial(F("AlertPusher: POST connected to %s"), serverDomain);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
    client.write(nextAlertString, strnlen(nextAlertString, sizeof(nextAlertString)));
    client.write('\r');
    client.write('\n');
  } else {
    serial(F("AlertPusher: POST connection to %s failed"), serverDomain);
    shouldSendHeadRequest = true;
  }
}
