/**
 * This class is a wrapper for Arduino's Serial interface.
 * The only function it supports is printf() and
 * Serial::println() will add a CRLF to the end of each line.
 */
#pragma once

#include <Arduino.h>

void serial(const __FlashStringHelper *format...);
void serial(const char *buffer);

class Serial_TC {
public:
  //  class methods
  static Serial_TC *instance();

  void vprintf(const __FlashStringHelper *format, va_list args);

private:
  //  class variable
  static Serial_TC *_instance;

  // instance variable
  bool printIsActive = false;

  //  instance methods
  Serial_TC();
};
