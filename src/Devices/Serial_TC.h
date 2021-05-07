/**
 * This class is a wrapper for Arduino's Serial interface.
 * The only function it supports is printf() and
 * Serial::println() will add a CRLF to the end of each line.
 */
#pragma once

#include <Arduino.h>

// The default Arduino library does not support printf() for float values.
// A workaround is to print two integers separated by a decimal point.
#define FLOAT(x, p) ((int)x), ((int)(fabs(x) * pow(10, p) + 0.5) % (int)pow(10, p))

void serial(const char *format...);
void serialWithTime(const char *format...);

class Serial_TC {
public:
  //  class methods
  static Serial_TC *instance();

  void vprintf(const char *format, va_list args);

private:
  //  class variable
  static Serial_TC *_instance;

  // instance variable
  bool printIsActive = false;

  //  instance methods
  Serial_TC();
};
