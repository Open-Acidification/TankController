#include "Devices/EthernetServer_TC.h"

#include <avr/wdt.h>

#include "DateTime_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/JSONBuilder.h"
#include "Devices/LiquidCrystal_TC.h"
#include "SD_TC.h"
#include "Serial_TC.h"
#include "TankController.h"

//  class variables
EthernetServer_TC* EthernetServer_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
EthernetServer_TC* EthernetServer_TC::instance() {
  if (!_instance) {
    _instance = new EthernetServer_TC(80);
  }
  return _instance;
}

//  instance methods
/**
 * Constructor: which port should we listen on?
 */
EthernetServer_TC::EthernetServer_TC(uint16_t port) : EthernetServer(port) {
  begin();
  IPAddress IP = Ethernet_TC::instance()->getIP();
  serial(F("Ethernet Server is listening on %i.%i.%i.%i:80"), IP[0], IP[1], IP[2], IP[3]);
}

void EthernetServer_TC::echo() {
  serial(F("echo() - \"%s\""), buffer + 19);
  int i = 19;
  while (buffer[i] != ' ' && buffer[i] != '\0') {
    ++i;
  }
  serial(F("echo() found space or null at %d"), i);
  if (memcmp_P(buffer + i - 3, F("%22"), 3)) {
    serial(F("bad"));
  } else {
    buffer[i - 3] = '\0';
    serial(F("echo \"%s\""), buffer + 19);
    sendHeadersWithSize(strnlen(buffer, sizeof(buffer)) - 19);
    client.write(buffer + 19);
    client.stop();
    state = NOT_CONNECTED;
  }
}

void EthernetServer_TC::current() {
  // get list of current values
  JSONBuilder builder;
  int size = builder.buildCurrentValues();
  char* text = builder.bufferPtr();
  // First send headers
  sendHeadersWithSize(size);
  // Write JSON file to client (will be null-terminated)
  client.write(text);
  client.stop();
  state = NOT_CONNECTED;
}

void EthernetServer_TC::display() {
  // First send headers
  sendHeadersWithSize(36);
  // get currently displayed lines
  client.write(LiquidCrystal_TC::instance()->getLine(0), 16);
  client.write('\r');
  client.write('\n');
  client.write(LiquidCrystal_TC::instance()->getLine(1), 16);
  client.write('\r');
  client.write('\n');
  client.stop();
  state = NOT_CONNECTED;
}

void EthernetServer_TC::keypress() {
  if (buffer[23] != ' ') {
    serial(F("value too long"));
    sendBadRequestHeaders();
  } else {
    // We have a one character keypress, check to see if valid character
    char key = buffer[22];
    if (key == '#' || key == '*' || (key >= '0' && key <= '9') || (key >= 'A' && key <= 'D')) {
      // States will handle keypresses appropriately
      TankController::instance()->setNextKey(key);
      sendRedirectHeaders();
    } else {
      serial(F("bad character: %c"), key);
      sendBadRequestHeaders();
    }
  }
  client.stop();
  state = NOT_CONNECTED;
}

bool EthernetServer_TC::file() {
  // Buffer has something like "GET /path HTTP/1.1"
  // and we want to put a null at the end of the path.
  int i = 4;
  while (buffer[i] != ' ') {
    ++i;
  }
  buffer[i] = '\0';
  // Look for file in SD card.
  if (!SD_TC::instance()->exists(buffer + 4)) {
    serial(F("file - \"%s\" not found!"), buffer + 4);
    return false;
  }
  // Open file and send headers with file size.
  File file = SD_TC::instance()->open(buffer + 4);
  uint32_t size = file.size();
  serial(F("file \"%s\" has a size of %lu"), buffer + 4, size);
  sendHeadersWithSize(size);
  // Send file contents
  uint32_t totalBytes = 0;
  uint32_t timeInRead = 0;
  uint32_t timeInWrite = 0;
  uint32_t timeInFlush = 0;

  wdt_disable();
  uint32_t flushCount = 0;
  while (file.available32()) {
    uint32_t startTime = millis();
    int readSize = file.read(buffer, sizeof(buffer));  // Flawfinder: ignore
    timeInRead += millis() - startTime;
    startTime = millis();
    int writeSize = client.write(buffer, readSize);
    timeInWrite += millis() - startTime;
    if (writeSize != readSize) {
      serial(F("totalBytes = %lu; read = %d; write = %d"), totalBytes, readSize, writeSize);
      break;
    }
    totalBytes += writeSize;
    if (totalBytes % 8196 == 0) {
      startTime = millis();
      client.flush();
      timeInFlush += millis() - startTime;
      ++flushCount;
    }
  }
  file.close();
  client.stop();
  state = NOT_CONNECTED;
  serial(F("write = %lu; freeMemory = %i"), totalBytes, TankController::instance()->freeMemory());
  serial(F("timeInRead = %lu; timeInWrite = %lu; timeInFlush = %lu"), timeInRead, timeInWrite, timeInFlush);
  wdt_enable(WDTO_8S);
  return true;
}

void EthernetServer_TC::get() {
  if (memcmp_P(buffer + 4, F("/echo?value=%22"), 15) == 0) {
    echo();
  } else if (memcmp_P(buffer + 4, F("/api/1/display"), 14) == 0) {
    display();
  } else if (memcmp_P(buffer + 4, F("/api/1/current"), 14) == 0) {
    current();
  } else if (!file()) {
    // TODO: send an error response
    serial(F("get \"%s\" not recognized!"), buffer + 4);
    sendBadRequestHeaders();
    client.stop();
    state = NOT_CONNECTED;
  }
}

void EthernetServer_TC::post() {
  if (memcmp_P(buffer + 5, F("/api/1/key?value="), 17) == 0) {
    keypress();
  } else if (!file()) {
    // TODO: send an error response
    serial(F("put \"%s\" not recognized!"), buffer + 5);
    client.stop();
    state = NOT_CONNECTED;
  }
}

void EthernetServer_TC::loop() {
  if (client || (client = accept())) {  // if we have a connection
    if (state == NOT_CONNECTED) {
      state = READ_REQUEST;
      bufferContentsSize = 0;
      connectedAt = millis();  // record start time (so we can do timeout)
    }
    // read request
    int next;
    while (state == READ_REQUEST && bufferContentsSize < sizeof(buffer) - 1 &&
           (next = client.read()) != -1) {  // Flawfinder: ignore
      buffer[bufferContentsSize++] = (char)(next & 0xFF);
      if (bufferContentsSize > 3 && (memcmp_P(buffer + bufferContentsSize - 4, F("\r\n\r\n"), 4) == 0)) {
        buffer[bufferContentsSize] = '\0';
        state = HAS_REQUEST;
        if (memcmp_P(buffer, F("GET "), 4) == 0) {
          state = GET_REQUEST;
          break;
        } else if (memcmp_P(buffer, F("POST "), 5) == 0) {
          state = POST_REQUEST;
          break;
        } else {
          serial(F("Bad or unsupported request"));
          sendBadRequestHeaders();
          state = BAD_REQUEST;
          break;
        }
      }
    }
    switch (state) {
      case GET_REQUEST:
        get();
        break;
      case POST_REQUEST:
        post();
        break;
      default:
        break;
    }
  } else if (state != NOT_CONNECTED) {  // existing connection has been closed
    state = NOT_CONNECTED;
    memset(buffer, 0, sizeof(buffer));
    bufferContentsSize = 0;
    client.stop();
    connectedAt = 0;
  } else {
    // no client and not recently connected
  }
}

void EthernetServer_TC::sendHeadersWithSize(uint32_t size) {
  char buffer[128];
  static const char response[] PROGMEM =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain;charset=UTF-8\r\n"
      "Content-Encoding: identity\r\n"
      "Content-Language: en-US\r\n";
  strncpy_P(buffer, (PGM_P)response, sizeof(buffer));
  client.write(buffer);
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Content-Length: %lu\r\n"), (unsigned long)size);
  client.write(buffer);

  // TODO: add "Date: " header
  // const __FlashStringHelper* weekdays[] = {F("Sun"), F("Mon"), F("Tue"), F("Wed"), F("Thu"), F("Fri"), F("Sat")};
  // const __FlashStringHelper* months[] = {F("Jan"), F("Feb"), F("Mar"), F("Apr"), F("May"), F("Jun"),
  //                                        F("Jul"), F("Aug"), F("Sep"), F("Oct"), F("Nov"), F("Dec")};
  // DateTime_TC now = DateTime_TC::now();
  // int weekday = weekday(now.getYear(), now.getMonth(), now.getDay());
  // snprintf_P(buffer, sizeof(buffer), F("Date: %s, %02d %s %04d %02d:%02d:%02d GMT\r\n"), );

  // blank line indicates end of headers
  client.write('\r');
  client.write('\n');
}

void EthernetServer_TC::sendRedirectHeaders() {
  char buffer[70];
  static const char response[] PROGMEM =
      "HTTP/1.1 303 See Other\r\n"
      "Location: /api/1/display\r\n"
      "\r\n";
  strncpy_P(buffer, (PGM_P)response, sizeof(buffer));
  client.write(buffer);
}

void EthernetServer_TC::sendBadRequestHeaders() {
  char buffer[30];
  static const char response[] PROGMEM = "HTTP/1.1 400 Bad Request\r\n\r\n";
  strncpy_P(buffer, (PGM_P)response, sizeof(buffer));
  client.write(buffer);
}

int EthernetServer_TC::weekday(int year, int month, int day) {
  // Calculate day of week in proleptic Gregorian calendar. Sunday == 0.
  int adjustment, mm, yy;
  if (year < 2000)
    year += 2000;
  adjustment = (14 - month) / 12;
  mm = month + 12 * adjustment - 2;
  yy = year - adjustment;
  return (day + (13 * mm - 1) / 5 + yy + yy / 4 - yy / 100 + yy / 400) % 7;
}
