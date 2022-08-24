#include "Devices/EthernetServer_TC.h"

#include <avr/wdt.h>

#include "DateTime_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/JSONBuilder.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Serial_TC.h"
#include "TankController.h"

//  class variables
EthernetServer_TC *EthernetServer_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
EthernetServer_TC *EthernetServer_TC::instance() {
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
  static const char boundary_P[] PROGMEM = "boundary";
  strncpy_P(boundary, (PGM_P)boundary_P, sizeof(boundary_P));
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
  } else if (isRequestForExistingFile()) {
    fileSetup();
  } else {
    serial(F("get \"%s\" not recognized!"), buffer + 4);
    sendResponse(HTTP_NOT_FOUND);
    state = FINISHED;
  }
}

// Handles an HTTP OPTIONS request
void EthernetServer_TC::options() {
  // Method not allowed
  sendResponse(HTTP_NOT_PERMITTED);
  state = FINISHED;
}

// Handles an HTTP POST request
void EthernetServer_TC::post() {
  if (memcmp_P(buffer + 6, F("api"), 3) == 0) {
    keypress();
  } else {
    serial(F("post \"%s\" not recognized!"), buffer + 6);
    sendResponse(HTTP_BAD_REQUEST);
    ;
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
      rootdir();
    } else if (memcmp_P(buffer + 11, F("testRead"), 8) == 0) {
      testReadSpeed();
    } else if (memcmp_P(buffer + 11, F("testWrite"), 9) == 0) {
      testWriteSpeed();
    } else {
      // Unimplemented in API 1
      serial(F("Request unimplemented in API 1"));
      sendResponse(HTTP_BAD_REQUEST);
      ;
      state = FINISHED;
    }
  } else {
    // Later API versions may be implemented here
    serial(F("unhandled API version"));
    sendResponse(HTTP_BAD_REQUEST);
    ;
    state = FINISHED;
  }
}

// Get list of current values
void EthernetServer_TC::current() {
  JSONBuilder builder;
  int size = builder.buildCurrentValues();
  char *text = builder.bufferPtr();
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
    sendResponse(HTTP_BAD_REQUEST);
  } else {
    // We have a one character keypress, check to see if valid character
    char key = buffer[22];
    if (key == '#' || key == '*' || (key >= '0' && key <= '9') || (key >= 'A' && key <= 'D')) {
      // States will handle keypresses appropriately
      TankController::instance()->setNextKey(key);
      sendResponse(HTTP_REDIRECT);
    } else {
      serial(F("bad character: %c"), key);
      sendResponse(HTTP_BAD_REQUEST);
    }
  }
  state = FINISHED;
}

// Non-member callback wrapper for singleton
void writeToClientBufferCallback(char *buffer, bool isFinished) {
  // The boolean value in the callback is true when the process is complete
  EthernetServer_TC::instance()->writeToClientBuffer(buffer, isFinished);
}

// Non-member callback wrapper for singleton
void countFilesCallback(int fileCount) {
  // Called when the count is complete
  EthernetServer_TC::instance()->sendHeadersForRootdir(fileCount);
}

// List the root directory to the client
void EthernetServer_TC::rootdir() {
  // Call function on SD Card
  // Provide callback to call when writing to the client buffer
  if (state == LISTING_FILES) {
    SD_TC::instance()->listRootToBuffer(writeToClientBufferCallback);
  } else if (state == COUNTING_FILES) {
#ifndef MOCK_PINS_COUNT
    SD_TC::instance()->countFiles(countFilesCallback);
#else
    countFilesCallback(0);
#endif
  } else {
    state = COUNTING_FILES;
    startTime = millis();
    serial(F("Preparing list of files in root directory..."));
  }
}

// Write to the client buffer
void EthernetServer_TC::writeToClientBuffer(char *buffer, bool isFinished) {
  // Write to client and return (ASSUME NULL-TERMINATED)
  client.write(buffer);
  if (isFinished) {
    client.write('\r');
    client.write('\n');
    int endTime = millis();
    serial(F("...Done sending, time = %i ms"), endTime - startTime);
    state = FINISHED;
  }
}

void EthernetServer_TC::sendHeadersForRootdir(int fileCount) {
#ifndef MOCK_PINS_COUNT
  serial(F("...%i files..."), fileCount);
  sendHeadersWithSize((uint32_t)fileCount * 24);  // 24 characters per line
  state = LISTING_FILES;
#else
  sendHeadersWithSize((uint32_t)49);
  state = LISTING_FILES;
#endif
}

void EthernetServer_TC::sdError() {
  sendResponse(HTTP_ERROR);
  state = FINISHED;
}

// Tests speed for reading a file from the SD Card
// Empirical results show about 1 ms per 512 B
void EthernetServer_TC::testReadSpeed() {
  wdt_disable();
  static const char path[] PROGMEM = "tstRdSpd.txt";
  char temp[sizeof(path)];
  strncpy_P(temp, (PGM_P)path, sizeof(temp));
  // Create the file and write garbage
  file = SD_TC::instance()->open(temp, O_RDWR | O_CREAT | O_AT_END);
  memset(buffer, ' ', sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';
  file.write(buffer);
  file.close();
  // Read 1 MB
  file = SD_TC::instance()->open(temp, O_RDONLY);
  int startTime = micros();
  file.read(buffer, sizeof(buffer));  // Flawfinder: ignore
  int endTime = micros();
  SD_TC::instance()->remove(temp);
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
bool EthernetServer_TC::isRequestForExistingFile() {
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
  sendHeadersWithSize(size);
  state = IN_TRANSFER;
  startTime = millis();
  fileContinue();
}

// Continue file transfer (return value is whether we are finished)
bool EthernetServer_TC::fileContinue() {
  if (file.available()) {
    int readSize = file.read(buffer, sizeof(buffer));  // Flawfinder: ignore
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
      case COUNTING_FILES:
        rootdir();
        break;
      case LISTING_FILES:
        rootdir();
        break;
      case NOT_CONNECTED:
        state = READ_REQUEST;
        connectedAt = millis();        // record start time (so we can do timeout)
        __attribute__((fallthrough));  // Mwahahaha, use switch statement fall-through in a good way!
      case READ_REQUEST:
        int next;
        while (bufferContentsSize < sizeof(buffer) - 1 && (next = client.read()) != -1) {  // Flawfinder: ignore
          buffer[bufferContentsSize++] = (char)(next & 0xFF);
          if (bufferContentsSize > 3 && (memcmp_P(buffer + bufferContentsSize - 4, F("\r\n\r\n"), 4) == 0)) {
            buffer[bufferContentsSize] = '\0';
            break;
          }
        }
        if (bufferContentsSize == 0) {
          if (millis() - connectedAt > TIMEOUT) {
            sendResponse(HTTP_TIMEOUT);
            state = FINISHED;
          }
        } else {
          if (memcmp_P(buffer, F("GET "), 4) == 0) {
            state = GET_REQUEST;
            get();
          } else if (memcmp_P(buffer, F("POST "), 5) == 0) {
            state = POST_REQUEST;
            post();
          } else if (memcmp_P(buffer, F("OPTIONS "), 8) == 0) {
            state = OPTIONS_REQUEST;
            options();
          } else {
            serial(buffer);
            serial(F("Unsupported request"));
            sendResponse(HTTP_NOT_IMPLEMENTED);
            state = FINISHED;
          }
        }
        break;
      default:
        serial(F("loop() - unknown state: %i\nReseting state"), (int)state);
        sendResponse(HTTP_ERROR);
        state = FINISHED;
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
}

void EthernetServer_TC::sendResponse(int code) {
  static const char response_303[] PROGMEM =
      "HTTP/1.1 303 See Other\r\n"
      "Location: /api/1/display\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "\r\n";
  static const char response_400[] PROGMEM =
      "HTTP/1.1 400 Bad Request\r\n"
      "\r\n";
  static const char response_404[] PROGMEM =
      "HTTP/1.1 404 Not Found\r\n"
      "\r\n";
  static const char response_405[] PROGMEM =
      "HTTP/1.1 405 Method Not Allowed\r\n"
      "Allow: GET, POST, HEAD\r\n"
      "\r\n";
  static const char response_408[] PROGMEM =
      "HTTP/1.1 408 Request Timeout\r\n"
      "Connection: close\r\n"
      "\r\n";
  static const char response_500[] PROGMEM =
      "HTTP/1.1 500 Internal Server Error\r\n"
      "\r\n";
  static const char response_501[] PROGMEM =
      "HTTP/1.1 501 Not Implemented\r\n"
      "\r\n";
  static const char *const response_table[] PROGMEM = {response_303, response_400, response_404, response_405,
                                                       response_408, response_501, response_500};
  char buffer[sizeof(response_303)];
  switch (code) {
    case HTTP_REDIRECT:
      strncpy_P(buffer, (char *)pgm_read_word(&(response_table[0])), sizeof(buffer));
      break;
    case HTTP_BAD_REQUEST:
      strncpy_P(buffer, (char *)pgm_read_word(&(response_table[1])), sizeof(buffer));
      break;
    case HTTP_NOT_FOUND:
      strncpy_P(buffer, (char *)pgm_read_word(&(response_table[2])), sizeof(buffer));
      break;
    case HTTP_NOT_PERMITTED:
      strncpy_P(buffer, (char *)pgm_read_word(&(response_table[3])), sizeof(buffer));
      break;
    case HTTP_TIMEOUT:
      strncpy_P(buffer, (char *)pgm_read_word(&(response_table[4])), sizeof(buffer));
      break;
    case HTTP_NOT_IMPLEMENTED:
      strncpy_P(buffer, (char *)pgm_read_word(&(response_table[5])), sizeof(buffer));
      break;
    default:
      strncpy_P(buffer, (char *)pgm_read_word(&(response_table[6])), sizeof(buffer));
  };
  client.write(buffer);
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
