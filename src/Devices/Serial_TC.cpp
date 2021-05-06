#include "Devices/Serial_TC.h"

#include "DateTime_TC.h"
#include "SD_TC.h"
#include "TC_util.h"

/**
 * global serial() functions
 */
void serial(const char *format...) {
  va_list args;
  va_start(args, format);
  Serial_TC::instance()->vprintf(format, args);
  va_end(args);
}
void serial(const __FlashStringHelper *format...) {
  va_list args;
  va_start(args, format);
  Serial_TC::instance()->vprintf((const char *)format, args);
  va_end(args);
}
void serialWithTime(const char *format...) {
  char buffer[100];
  unsigned long ms = millis();
  DateTime_TC now = DateTime_TC::now();
  snprintf(buffer, sizeof(buffer), "Timestamp of next line: YYYY/MM/DD hh:mm:ss.%03i", (int)ms % 1000);
  now.toString(buffer);
  serial(buffer);
  va_list args;
  va_start(args, format);
  Serial_TC::instance()->vprintf(format, args);
  va_end(args);
}
void serialWithTime(const __FlashStringHelper *format...) {
  char buffer[100];
  unsigned long ms = millis();
  DateTime_TC now = DateTime_TC::now();
  snprintf(buffer, sizeof(buffer), "Timestamp of next line: YYYY/MM/DD hh:mm:ss.%03i", (int)ms % 1000);
  now.toString(buffer);
  serial(buffer);
  va_list args;
  va_start(args, format);
  Serial_TC::instance()->vprintf((const char *)format, args);
  va_end(args);
}

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
 * printf() uses a variant of snprintf() so supports the expected formats
 */
void Serial_TC::vprintf(const char *format, va_list args) {
  char buffer[256];
  vsnprintf(buffer, sizeof(buffer), format, args);
  Serial.println(buffer);
  Serial.flush();
  // need to avoid recursion since SD_TC could call serial()
  if (!printIsActive) {
    printIsActive = true;
    // this seems to cause problems on the actual hardware
    // SD_TC::instance()->appendToSerialLog(buffer);    // JGF
    printIsActive = false;
  }
}
