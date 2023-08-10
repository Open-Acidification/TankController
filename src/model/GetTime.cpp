#include "model/GetTime.h"

#include "TankController.h"
#include "model/TC_util.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/Ethernet_TC.h"
#include "wrappers/Serial_TC.h"

//  class variables
GetTime* GetTime::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
GetTime* GetTime::instance(int tzOffsetHrs) {
  if (!_instance) {
    _instance = new GetTime(tzOffsetHrs);
  }
  return _instance;
}

//  instance methods
GetTime::GetTime(int tzOffsetHrs) {
  this->buffer[0] = '\0';
  this->tzOffsetHrs = tzOffsetHrs;
}

void GetTime::loop() {
  if (!(Ethernet_TC::instance()->getIsUsingDHCP())) {
    return;
  }
  // is it time to send ?
  unsigned long now = millis();
  if (now >= nextSendTime) {
    sendRequest();
    // jump to the next multiple of interval
    nextSendTime = (now / interval + 1) * interval;
  }
  // are we still connected?
  if (client.connected()) {
    if (client.available()) {  // bytes are remaining in the current packet
      int next;
      while ((next = client.read()) != -1) {  // Flawfinder: ignore
        if (next) {
          if (next == '\r') {
            buffer[index] = '\0';
            if (index > 6 && memcmp("Date: ", buffer, 6) == 0) {
              // "Date: Wed, 19 Jul 2023 04:18:52 GMT"
              uint8_t day = ((buffer[11] - '0') * 10) + (buffer[12] - '0');
              uint8_t year = ((buffer[20] - '0') * 10) + (buffer[21] - '0');
              uint8_t hour = ((buffer[23] - '0') * 10) + (buffer[24] - '0');
              uint8_t min = ((buffer[26] - '0') * 10) + (buffer[27] - '0');
              uint8_t sec = ((buffer[29] - '0') * 10) + (buffer[30] - '0');
              uint8_t month = 0;
              if (memcmp_P(F("Jan"), buffer + 14, 3) == 0) {
                month = 1;
              } else if (memcmp_P(F("Feb"), buffer + 14, 3) == 0) {
                month = 2;
              } else if (memcmp_P(F("Mar"), buffer + 14, 3) == 0) {
                month = 3;
              } else if (memcmp_P(F("Apr"), buffer + 14, 3) == 0) {
                month = 4;
              } else if (memcmp_P(F("May"), buffer + 14, 3) == 0) {
                month = 5;
              } else if (memcmp_P(F("Jun"), buffer + 14, 3) == 0) {
                month = 6;
              } else if (memcmp_P(F("Jul"), buffer + 14, 3) == 0) {
                month = 7;
              } else if (memcmp_P(F("Aug"), buffer + 14, 3) == 0) {
                month = 8;
              } else if (memcmp_P(F("Sep"), buffer + 14, 3) == 0) {
                month = 9;
              } else if (memcmp_P(F("Oct"), buffer + 14, 3) == 0) {
                month = 10;
              } else if (memcmp_P(F("Nov"), buffer + 14, 3) == 0) {
                month = 11;
              } else if (memcmp_P(F("Dec"), buffer + 14, 3) == 0) {
                month = 12;
              }
              DateTime dt = DateTime(year + 2000, month, day, hour, min, sec);
              dt = dt + TimeSpan(tzOffsetHrs * 60 * 60);
              DateTime_TC newTime = DateTime_TC(dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());
              newTime.setAsCurrent();
              serial(F("Time updated to %s"), DateTime_TC::now().as16CharacterString());
            }
          } else if (next == '\n') {
            index = 0;
            buffer[index] = '\0';
          } else {
            buffer[index++] = next;
          }
        }
      }
    } else {
    }
  } else {
    // will this close every 15 ms?
    client.stop();
  }
}

void GetTime::sendRequest() {
  static const char format[] PROGMEM =
      "GET /Date HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Connection: close\r\n"
      "\r\n";
  snprintf_P(buffer, sizeof(buffer), (PGM_P)format, serverDomain);
  if (client.connected() || client.connect(serverDomain, 80)) {
    serial(F("connected to %s"), serverDomain);
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
  } else {
    serial(F("connection to %s failed"), serverDomain);
  }
  buffer[0] = '\0';
  index = 0;
}
