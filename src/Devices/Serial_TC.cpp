#include "Devices/Serial_TC.h"

#include <cstring>

#include "DateTime_TC.h"
#include "SD_TC.h"
#include "TC_util.h"

/**
 * static variable for singleton
 */
Serial_TC *Serial_TC::_instance = nullptr;

/**
 * static member function to return singleton
 */
Serial_TC *Serial_TC::instance() {
  if (!_instance) {
    _instance = new Serial_TC();
  }
  return _instance;
}

/**
 * constructor (private so clients use the singleton)
 */
Serial_TC::Serial_TC() {
  Serial.begin(9600);
}

/**
 * printf() uses a variant of sprintf() so supports the expected formats
 */
void Serial_TC::printf(const char *format, ...) {
  va_list arguments;
  char buffer[256];
  va_start(arguments, format);
  vsnprintf(buffer, sizeof(buffer), format, arguments);
  va_end(arguments);
  Serial.println(buffer);
  SD_TC::instance()->appendToSerialLog(buffer);
}

/**
 * ts_printf() prints a timestamp first
 */
void Serial_TC::ts_printf(const char *format, ...) {
  char buffer[1000];
  unsigned long ms = millis();
  DateTime_TC now = DateTime_TC::now();
  sprintf(buffer, "Timestamp of next line: YYYY/MM/DD hh:mm:ss.%03i", (int)ms % 1000);
  now.toString(buffer);
  printf(buffer);
  va_list arguments;
  printf(format, arguments);
  va_end(arguments);
}
