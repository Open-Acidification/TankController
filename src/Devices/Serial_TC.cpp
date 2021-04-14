#include "Devices/Serial_TC.h"

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
  char buffer[1000];
  va_start(arguments, format);
  vsnprintf(buffer, sizeof(buffer), format, arguments);
  va_end(arguments);
  Serial.println(buffer);
}
