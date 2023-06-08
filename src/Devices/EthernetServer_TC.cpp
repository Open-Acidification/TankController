#include "Devices/EthernetServer_TC.h"

#include <avr/wdt.h>

#include "DateTime_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/JSONBuilder.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"
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
  const __FlashStringHelper *boundary_P = F("boundary");
  // TODO: A long string of apparently random characters should be used as the boundary instead,
  // because they would be less likely to be part of the message
  strscpy_P(boundary, boundary_P, sizeof(boundary));
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

// Handles an HTTP GET request
void EthernetServer_TC::get() {
  if (memcmp_P(buffer + 4, F("/ "), 2) == 0) {
    sendHomeRedirect();
    state = FINISHED;
  } else 
  if (memcmp_P(buffer + 5, F("echo"), 4) == 0) {
    echo();
  } else if (memcmp_P(buffer + 5, F("api"), 3) == 0) {
    getApiHandler();
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
  const __FlashStringHelper *response =
      F("HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain;charset=UTF-8\r\n"
        "Content-Encoding: identity\r\n"
        "Content-Language: en-US\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: OPTIONS, GET, HEAD, POST, PUT\r\n"
        "\r\n");
  strscpy_P(buffer, response, sizeof(buffer));
  client.write(buffer);

  state = FINISHED;
}

// Handles an HTTP POST request
void EthernetServer_TC::post() {
  if (memcmp_P(buffer + 6, F("api/1/key?value="), 16) == 0) {
    keypress();
  } else {
    serial(F("post \"%s\" not recognized!"), buffer + 6);
    sendResponse(HTTP_BAD_REQUEST);
    state = FINISHED;
  }
}

// Handles an HTTP PUT request
void EthernetServer_TC::put() {
  enum { Kd, Ki, Kp } var;
  if (memcmp_P(buffer + 4, F("/api/1/data?Kd="), 15) == 0) {
    var = Kd;
  } else if (memcmp_P(buffer + 4, F("/api/1/data?Ki="), 15) == 0) {
    var = Ki;
  } else if (memcmp_P(buffer + 4, F("/api/1/data?Kp="), 15) == 0) {
    var = Kp;
  } else {
    serial(F("put \"%s\" not recognized!"), buffer + 5);
    sendResponse(HTTP_BAD_REQUEST);
    state = FINISHED;
    return;
  }
  float value = strtofloat(buffer + 19);
  switch (var) {
    case Kd:
      PID_TC::instance()->setKd(value);
      break;
    case Ki:
      PID_TC::instance()->setKi(value);
      break;
    case Kp:
      PID_TC::instance()->setKp(value);
      break;
  }
  sendCurrentRedirect();
  state = FINISHED;
}

/* API Handler
 * currently only version 1 is supported
 * Calls helper functions
 */

// API for certain get and post requests
void EthernetServer_TC::getApiHandler() {
  if (buffer[9] == '1') {
    // API version 1
    // When you add a new API, keep this for backwards compatibility
    if (memcmp_P(buffer + 11, F("data"), 4) == 0) {
      currentData();
    } else if (memcmp_P(buffer + 11, F("display"), 7) == 0) {
      display();
    } else if (memcmp_P(buffer + 11, F("rootdir"), 7) == 0) {
      rootdirSetup();
    } else if (memcmp_P(buffer + 11, F("testRead"), 8) == 0) {
      testReadSpeed();
    } else if (memcmp_P(buffer + 11, F("testWrite"), 9) == 0) {
      testWriteSpeed();
    } else {
      // Unimplemented in API 1
      serial(F("Request unimplemented in API 1"));
      sendResponse(HTTP_BAD_REQUEST);
      state = FINISHED;
    }
  } else {
    // Later API versions may be implemented here
    serial(F("unhandled API version"));
    sendResponse(HTTP_BAD_REQUEST);
    state = FINISHED;
  }
}

// Get list of current values
void EthernetServer_TC::currentData() {
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
      sendDisplayRedirect();
    } else {
      serial(F("bad character: %c"), key);
      sendResponse(HTTP_BAD_REQUEST);
    }
  }
  state = FINISHED;
}

// Non-member callback wrapper for singleton
void writeToClientBufferCallback(const char *buffer, bool isFinished) {
  // The boolean value in the callback is true when the process is complete
  EthernetServer_TC::instance()->writeToClientBuffer(buffer, isFinished);
}

// Non-member callback wrapper for singleton
void countFilesCallback(int fileCount) {
  // Called when the count is complete
  EthernetServer_TC::instance()->sendHeadersForRootdir(fileCount);
}

// Count files in root directory so that the Content-Length
// for the header may be calculated
void EthernetServer_TC::rootdirSetup() {
  if (state == COUNTING_FILES) {
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
    countFilesCallback(0);
#else
    if (!SD_TC::instance()->countFiles(countFilesCallback)) {
      sendResponse(HTTP_ERROR);
      state = FINISHED;
    };
#endif
  } else {
    state = COUNTING_FILES;
    startTime = millis();
    serial(F("Preparing list of files in root directory..."));
  }
}

// List the root directory to the client
void EthernetServer_TC::rootdir() {
  // Call function on SD Card
  // Provide callback to call when writing to the client buffer
  if (!SD_TC::instance()->listRootToBuffer(writeToClientBufferCallback)) {
    serial(F("listRootToBuffer() failed"));
    sendResponse(HTTP_ERROR);
    state = FINISHED;
  };
}

// Write to the client buffer
void EthernetServer_TC::writeToClientBuffer(const char *buffer, bool isFinished) {
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
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  sendHeadersWithSize((uint32_t)49);
#else
  serial(F("...%i files..."), fileCount);
  sendHeadersWithSize((uint32_t)fileCount * 24);  // 24 characters per line
#endif
  state = LISTING_FILES;
}

// Tests speed for reading a file from the SD Card
// Empirical results show about 1.28 ms per 512 B
void EthernetServer_TC::testReadSpeed() {
  wdt_disable();
  const __FlashStringHelper *path = F("tstRdSpd.txt");
  char temp[15];
  strscpy_P(temp, path, sizeof(temp));
  // Create the file and write garbage
  file = SD_TC::instance()->open(temp, O_RDWR | O_CREAT | O_AT_END);
  memset(buffer, ' ', sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';
  file.write(buffer);
  file.close();
  // Read 1 MB
  file = SD_TC::instance()->open(temp, O_RDONLY);
  long startTime = micros();
  file.read(buffer, sizeof(buffer));  // Flawfinder: ignore
  long endTime = micros();
  SD_TC::instance()->remove(temp);
  serial(F("Time reading %i bytes: %i us"), sizeof(buffer), (endTime - startTime));
  wdt_enable(WDTO_8S);
  state = FINISHED;
}

// Tests speed for writing to client buffer
// Empirical results show about 6.43 ms per 512 B
void EthernetServer_TC::testWriteSpeed() {
  wdt_disable();
  char buffer[512];
  memset(buffer, ' ', 511);
  buffer[511] = '\0';
  for (int i = 0; i < 10; ++i) {
    long startTime = micros();
    client.write(buffer);
    long endTime = micros();
    serial(F("Time writing %i bytes: %i us"), sizeof(buffer), (endTime - startTime));
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
      // TODO: This is an infinite loop (though non-blocking)
      // Consider: Close file, change state, return true?
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
        rootdirSetup();
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
          // serial(F("HTTP Request: \"%s\""), buffer);
          if (memcmp_P(buffer, F("GET "), 4) == 0) {
            state = GET_REQUEST;
            get();
          } else if (memcmp_P(buffer, F("POST "), 5) == 0) {
            state = POST_REQUEST;
            post();
          } else if (memcmp_P(buffer, F("PUT "), 4) == 0) {
            state = PUT_REQUEST;
            put();
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
  const __FlashStringHelper *response =
      F("HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain;charset=UTF-8\r\n"
        "Content-Encoding: identity\r\n"
        "Content-Language: en-US\r\n"
        "Access-Control-Allow-Origin: *\r\n");
  char buffer[200];
  strscpy_P(buffer, response, sizeof(buffer));
  client.write(buffer);
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Content-Length: %lu\r\n"), (unsigned long)size);
  client.write(buffer);

  // TODO: add "Date: " header
  // const PROGMEM __FlashStringHelper* weekdays[] = {F("Sun"), F("Mon"), F("Tue"), F("Wed"), F("Thu"), F("Fri"),
  // F("Sat")}; const PROGMEM __FlashStringHelper* months[] = {F("Jan"), F("Feb"), F("Mar"), F("Apr"), F("May"),
  // F("Jun"),
  //                                        F("Jul"), F("Aug"), F("Sep"), F("Oct"), F("Nov"), F("Dec")};
  // DateTime_TC now = DateTime_TC::now();
  // int weekday = weekday(now.getYear(), now.getMonth(), now.getDay());
  // snprintf_P(buffer, sizeof(buffer), F("Date: %s, %02d %s %04d %02d:%02d:%02d GMT\r\n"), );

  // blank line indicates end of headers
  client.write('\r');
  client.write('\n');
}

void EthernetServer_TC::sendCurrentRedirect() {
  const __FlashStringHelper *response_303 =
      F("HTTP/1.1 303 See Other\r\n"
        "Location: /api/1/data\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n");
  strscpy_P(buffer, response_303, sizeof(buffer));
  client.write(buffer);
}

void EthernetServer_TC::sendDisplayRedirect() {
  const __FlashStringHelper *response_303 =
      F("HTTP/1.1 303 See Other\r\n"
        "Location: /api/1/display\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n");
  strscpy_P(buffer, response_303, sizeof(buffer));
  client.write(buffer);
}

void EthernetServer_TC::sendHomeRedirect() {
  const __FlashStringHelper *response_303 =
      F("HTTP/1.1 303 See Other\r\n"
        "Location: https://open-acidification.github.io/TankControllerManager/\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n");
  strscpy_P(buffer, response_303, sizeof(buffer));
  client.write(buffer);
}

void EthernetServer_TC::sendResponse(int code) {
  const __FlashStringHelper *response_400 =
      F("HTTP/1.1 400 Bad Request\r\n"
        "\r\n");
  const __FlashStringHelper *response_404 =
      F("HTTP/1.1 404 Not Found\r\n"
        "\r\n");
  const __FlashStringHelper *response_405 =
      F("HTTP/1.1 405 Method Not Allowed\r\n"
        "Allow: GET, POST\r\n"
        "\r\n");
  const __FlashStringHelper *response_408 =
      F("HTTP/1.1 408 Request Timeout\r\n"
        "Connection: close\r\n"
        "\r\n");
  const __FlashStringHelper *response_500 =
      F("HTTP/1.1 500 Internal Server Error\r\n"
        "\r\n");
  const __FlashStringHelper *response_501 =
      F("HTTP/1.1 501 Not Implemented\r\n"
        "\r\n");
  char buffer[100];  // Space for longest of above responses
  switch (code) {
    case HTTP_BAD_REQUEST:
      strscpy_P(buffer, response_400, sizeof(buffer));
      break;
    case HTTP_NOT_FOUND:
      strscpy_P(buffer, response_404, sizeof(buffer));
      break;
    case HTTP_NOT_PERMITTED:
      strscpy_P(buffer, response_405, sizeof(buffer));
      break;
    case HTTP_TIMEOUT:
      strscpy_P(buffer, response_408, sizeof(buffer));
      break;
    case HTTP_NOT_IMPLEMENTED:
      strscpy_P(buffer, response_501, sizeof(buffer));
      break;
    default:
      strscpy_P(buffer, response_500, sizeof(buffer));
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
