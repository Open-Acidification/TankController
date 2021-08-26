#include "Devices/EthernetServer_TC.h"

#include <avr/wdt.h>

#include "DateTime_TC.h"
#include "SD_TC.h"
#include "Serial_TC.h"
#include "TankControllerLib.h"

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
  serial("Ethernet Server is listening on port %d", port);
}

void EthernetServer_TC::echo() {
  serial("echo() - \"%s\"", buffer + 19);
  int i = 19;
  while (buffer[i] != ' ' && buffer[i] != '\0') {
    ++i;
  }
  serial("echo() found space or null at %d", i);
  if (memcmp(buffer + i - 3, "%22", 3)) {
    serial("bad");
  } else {
    buffer[i - 3] = '\0';
    serial("echo \"%s\"", buffer + 19);
    sendHeadersWithSize(strlen(buffer + 19));
    client.write(buffer + 19);
    client.stop();
    state = NOT_CONNECTED;
  }
}

bool EthernetServer_TC::file() {
  int i = 4;
  while (buffer[i] != ' ') {
    ++i;
  }
  buffer[i] = '\0';
  if (!SD_TC::instance()->exists(buffer + 4)) {
    serial("file - \"%s\" not found!", buffer + 4);
    return false;
  }
  File file = SD_TC::instance()->open(buffer + 4);
  uint32_t size = file.size();
  serial("file \"%s\" has a size of %lu", buffer + 4, size);
  sendHeadersWithSize(size);
  uint32_t total = 0;
  wdt_disable();
  uint32_t start = millis();
  uint32_t flushCount = 0;
  uint32_t flushTime = 0;
  while (file.available32()) {
    int readSize = file.read(buffer, sizeof(buffer));
    int writeSize = client.write(buffer, readSize);
    if (writeSize != readSize) {
      serial("total = %lu; read = %d; write = %d", total, readSize, writeSize);
      break;
    }
    total += writeSize;
    if (total % 8196 == 0) {
      uint32_t before = millis();
      client.flush();
      uint32_t after = millis();
      ++flushCount;
      flushTime += after - before;
      // serial("total = %lu; flush took %lu millis", total, after - before);
      // delay(10);
    }
  }
  uint32_t end = millis();
  file.close();
  client.stop();
  state = NOT_CONNECTED;
  serial("write = %lu; freeMemory = %i", total, TankControllerLib::instance()->freeMemory());
  serial("time = %lu; average flush time = %lu", end - start, (uint32_t)flushTime / flushCount);
  wdt_enable(WDTO_8S);
  return true;
}

void EthernetServer_TC::get() {
  if (memcmp(buffer + 4, "/echo?value=%22", 15) == 0) {
    echo();
  } else if (!file()) {
    // TODO: send an error response
    serial("get \"%s\" not recognized!", buffer + 4);
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
    while (state == READ_REQUEST && bufferContentsSize < sizeof(buffer) && (next = client.read()) != -1) {
      buffer[bufferContentsSize++] = (char)(next & 0xFF);
      if (bufferContentsSize > 1 && buffer[bufferContentsSize - 2] == '\r' && buffer[bufferContentsSize - 1] == '\n') {
        buffer[bufferContentsSize - 2] = '\0';
        state = HAS_REQUEST;
        if (memcmp(buffer, "GET ", 4) == 0) {
          state = GET_REQUEST;
          break;
        }
        break;
      }
    }
    switch (state) {
      case GET_REQUEST:
        get();
        break;
      default:
        break;
    }
  } else if (state != NOT_CONNECTED) {  // existing connection has been closed
    state = NOT_CONNECTED;
    bufferContentsSize = 0;
    client.stop();
    connectedAt = 0;
  } else {
    // no client and not recently connected
  }
}

void EthernetServer_TC::sendHeadersWithSize(uint32_t size) {
  const char response[] =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain;charset=UTF-8\r\n"
      "Content-Encoding: identity\r\n"
      "Content-Language: en-US\r\n";
  client.write(response);
  char buffer[40];
  snprintf(buffer, sizeof(buffer), "Content-Length: %lu\r\n", (unsigned long)size);
  client.write(buffer);

  // TODO: add "Date:"" header
  const char* weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  DateTime_TC now = DateTime_TC::now();
  // int weekday = weekday(now.getYear(), now.getMonth(), now.getDay());
  // snprintf(buffer, sizeof(buffer), "Date: %s, %02d %s %04d %02d:%02d:%02d GMT\r\n", );

  // blank line indicates end of headers
  client.write("\r\n");
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
