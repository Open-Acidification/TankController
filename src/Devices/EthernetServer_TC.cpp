#include "Devices/EthernetServer_TC.h"

#include <avr/wdt.h>

#include "DateTime_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/JSONBuilder.h"
#include "Devices/LiquidCrystal_TC.h"
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
  static const char boundary_P[] PROGMEM = "boundary";
  memcpy(boundary, (PGM_P)boundary_P, sizeof(boundary_P));
  serial(F("Ethernet Server is listening on %i.%i.%i.%i:80"), IP[0], IP[1], IP[2], IP[3]);
}

// echo() - Proof of concept for the EthernetServer
void EthernetServer_TC::echo() {
  serial(F("echo() - \"%s\""), buffer + 19);
  int i = 19;
  while (buffer[i] != ' ' && buffer[i] != '\0') {
    ++i;
  }
  serial(F("echo() found space or null at %d"), i);
  if (memcmp_P(buffer + i - 3, F("%22"), 3)) {
    serial(F("bad"));
    state = FINISHED;
  } else {
    buffer[i - 3] = '\0';
    serial(F("echo \"%s\""), buffer + 19);
    sendHeadersWithSize(strnlen(buffer, sizeof(buffer)) - 19);
    client.write(buffer + 19);
    state = FINISHED;
  }
}

/* get() and post()
 * For handling respective HTTP requests
 */

// Handles an HTTP GET request
void EthernetServer_TC::get() {
  if (memcmp_P(buffer + 5, F("echo"), 4) == 0) {
    echo();
  } else if (memcmp_P(buffer + 5, F("api"), 3) == 0) {
    apiHandler();
  } else if (isFileRequest()) {
    fileSetup();
  } else {
    serial(F("get \"%s\" not recognized!"), buffer + 4);
    sendBadRequestHeaders();
    state = FINISHED;
  }
}

// Handles an HTTP POST request
void EthernetServer_TC::post() {
  if (memcmp_P(buffer + 6, F("api"), 3) == 0) {
    keypress();
  } else {
    serial(F("post \"%s\" not recognized!"), buffer + 6);
    state = FINISHED;
  }
}

/* API Handler
 * currently only version 1 is supported
 * Calls helper functions
 */

// API for certain get and post requests
void EthernetServer_TC::apiHandler() {
  if (buffer[9] == '1') {
    // API version 1
    // Keep this for backwards compatibility
    if (memcmp_P(buffer + 11, F("current"), 7) == 0) {
      current();
    } else if (memcmp_P(buffer + 11, F("display"), 7) == 0) {
      display();
    } else if (memcmp_P(buffer + 11, F("rootdir"), 7) == 0) {
      state = IN_PROGRESS;
      rootdir();
    } else if (memcmp_P(buffer + 11, F("testRead"), 8) == 0) {
      testReadSpeed();
    } else if (memcmp_P(buffer + 11, F("testWrite"), 9) == 0) {
      testWriteSpeed();
    } else {
      // Unimplemented in API 1
      serial(F("Request unimplemented in API 1"));
      sendBadRequestHeaders();
      state = FINISHED;
    }
  } else {
    // Later API versions may be implemented here
    serial(F("unhandled API version"));
    sendBadRequestHeaders();
    state = FINISHED;
  }
}

// Get list of current values
void EthernetServer_TC::current() {
  JSONBuilder builder;
  int size = builder.buildCurrentValues();
  char* text = builder.bufferPtr();
  // First send headers
  sendHeadersWithSize(size);
  // Write JSON file to client (will be null-terminated)
  client.write(text);
  client.write('\r');
  client.write('\n');
  state = FINISHED;
}

// Get current display on the LCD
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
  state = FINISHED;
}

// Input a key to the tank controller keypad
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
  state = FINISHED;
}

// Non-member callback wrapper for singleton
void writeToClientBufferCallback(char* buffer, bool isFinished) {
  // The boolean value in the callback is true when the process is complete
  EthernetServer_TC::instance()->writeToClientBuffer(buffer, isFinished);
}

// List the root directory to the client
void EthernetServer_TC::rootdir() {
  // Call function on SD Card
  // Provide callback to call when writing to the client buffer
  SD_TC::instance()->listRootToBuffer(writeToClientBufferCallback);
}

// Write to the client buffer
void EthernetServer_TC::writeToClientBuffer(char* buffer, bool isFinished) {
  // Write to client and return (ASSUME NULL-TERMINATED)
  client.write(buffer);
  if (isFinished) {
    client.write('\r');
    client.write('\n');
    state = FINISHED;
  }
}

// Tests speed for reading a file from the SD Card
// Empirical results show about 12 us per 512 B
void EthernetServer_TC::testReadSpeed() {
  wdt_disable();
  static const char path[] PROGMEM = "tstRdSpd.txt";
  char temp[sizeof(path)];
  strncpy_P(temp, (PGM_P)path, sizeof(temp));
  // Create the file and write garbage
  file = SD_TC::instance()->open(temp, FILE_WRITE);
  memset(buffer, ' ', 512);
  buffer[511] = '\0';
  file.println(buffer);
  file.close();
  // Read 1 MB
  file = SD_TC::instance()->open(temp, O_READ | O_WRITE);
  int startTime = micros();
  file.read(buffer, 512);
  int endTime = micros();
  file.remove();
  serial(F("Time reading 512B: %i us"), (endTime - startTime));
  wdt_enable(WDTO_8S);
  state = FINISHED;
}

// Tests speed for writing to client buffer
// Empirical results show about 6 ms per 512 B
void EthernetServer_TC::testWriteSpeed() {
  wdt_disable();
  char buffer[512];
  memset(buffer, ' ', 511);
  buffer[511] = '\0';
  for (int i = 0; i < 10; ++i) {
    int startTime = micros();
    client.write(buffer);
    int endTime = micros();
    serial(F("Time writing 512B to client: %i us"), (endTime - startTime));
  }
  wdt_enable(WDTO_8S);
  state = FINISHED;
}

// Handles a get request with a path
bool EthernetServer_TC::isFileRequest() {
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
  } else {
    return true;
  }
}

// Open file and send headers with file size.
void EthernetServer_TC::fileSetup() {
  file = SD_TC::instance()->open(buffer + 4);
  uint32_t size = file.size();
  serial(F("file \"%s\" has a size of %lu"), buffer + 4, size);
  sendFileHeadersWithSize(size);
  state = IN_TRANSFER;
  startTime = millis();
  fileContinue();
}

// Continue file transfer (return value is whether we are finished)
bool EthernetServer_TC::fileContinue() {
  client.write(boundary);
  if (file.available32()) {
    int readSize = file.read(buffer, sizeof(buffer));
    int writeSize = client.write(buffer, readSize);
    if (writeSize != readSize) {
      serial(F("read = %d; write = %d"), readSize, writeSize);
    }
    return false;
  } else {
    int endTime = millis();
    serial(F("Done sending file, time = %i ms"), endTime - startTime);
    file.close();
    state = FINISHED;
    return true;
  }
}

// Main loop called by TankController::loop()
void EthernetServer_TC::loop() {
  if (state == FINISHED) {  // Tear down
    state = NOT_CONNECTED;
    memset(buffer, 0, sizeof(buffer));
    bufferContentsSize = 0;
    connectedAt = 0;
    client.stop();
    return;
  }
  if (client || (client = accept())) {  // if we have a connection
    switch (state) {
      case IN_TRANSFER:
        if (fileContinue()) {
          state = FINISHED;
        }
        break;
      case IN_PROGRESS:
        // In progress (so far only for SD Card)
        rootdir();
        break;
      case NOT_CONNECTED:
        state = READ_REQUEST;
        connectedAt = millis();  // record start time (so we can do timeout)
      // Mwahahaha, use switch statement fall-through in a good way!
      case READ_REQUEST:
        int next;
        while (bufferContentsSize < sizeof(buffer) - 1 && (next = client.read()) != -1) {  // Flawfinder: ignore
          buffer[bufferContentsSize++] = (char)(next & 0xFF);
          if (bufferContentsSize > 3 && (memcmp_P(buffer + bufferContentsSize - 4, F("\r\n\r\n"), 4) == 0)) {
            buffer[bufferContentsSize] = '\0';
            break;
          }
        }
        if (bufferContentsSize > 0) {
          if (memcmp_P(buffer, F("GET "), 4) == 0) {
            state = GET_REQUEST;
            get();
            break;
          } else if (memcmp_P(buffer, F("POST "), 5) == 0) {
            state = POST_REQUEST;
            post();
            break;
          } else {
            serial(buffer);
            serial(F("Bad or unsupported request"));
            sendBadRequestHeaders();
            state = FINISHED;
            break;
          }
        }
      default:
        break;
    }
  } else if (state != NOT_CONNECTED) {  // In case client disconnects early
    state = FINISHED;
  } else {
    // no client and not recently connected
  }
}

/* Header helper functions
 * Needs work, separate by HTTP response type
 */

// 200 response with a content size
void EthernetServer_TC::sendHeadersWithSize(uint32_t size) {
  static const char response[] PROGMEM =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain;charset=UTF-8\r\n"
      "Content-Encoding: identity\r\n"
      "Content-Language: en-US\r\n"
      "Access-Control-Allow-Origin: *\r\n";
  char buffer[sizeof(response)];
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
  state = FINISHED;
}

void EthernetServer_TC::sendFileHeadersWithSize(uint32_t size) {
  snprintf_P(buffer, sizeof(buffer),
             (PGM_P)F("HTTP/1.1 200 OK\r\n"
                      "Content-Type: multipart/form-data; boundary=%s\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "Content-Length: %lu\r\n"),
             boundary, (unsigned long)size);
  client.write(buffer);

  // blank line indicates end of headers
  client.write('\r');
  client.write('\n');
  state = FINISHED;
}

// 303 response
void EthernetServer_TC::sendRedirectHeaders() {
  static const char response[] PROGMEM =
      "HTTP/1.1 303 See Other\r\n"
      "Location: /api/1/display\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "\r\n";
  char buffer[sizeof(response)];
  strncpy_P(buffer, (PGM_P)response, sizeof(buffer));
  client.write(buffer);
  state = FINISHED;
}

// 400 response
void EthernetServer_TC::sendBadRequestHeaders() {
  char buffer[30];
  static const char response[] PROGMEM = "HTTP/1.1 400 Bad Request\r\n\r\n";
  strncpy_P(buffer, (PGM_P)response, sizeof(buffer));
  client.write(buffer);
  state = FINISHED;
}

// Calculate day of week in proleptic Gregorian calendar. Sunday == 0.
int EthernetServer_TC::weekday(int year, int month, int day) {
  int adjustment, mm, yy;
  if (year < 2000)
    year += 2000;
  adjustment = (14 - month) / 12;
  mm = month + 12 * adjustment - 2;
  yy = year - adjustment;
  return (day + (13 * mm - 1) / 5 + yy + yy / 4 - yy / 100 + yy / 400) % 7;
}
