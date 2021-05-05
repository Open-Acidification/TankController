/**
 * This class is a wrapper for Arduino's Serial interface.
 * The only function it supports is printf() and
 * Serial::println() will add a CRLF to the end of each line.
 */
#pragma once

#include <Arduino.h>

// helpful macros
#define serial(...) Serial_TC::instance()->printf(__VA_ARGS__)
#define serialWithTime(...) Serial_TC::instance()->ts_printf(__VA_ARGS__)

class Serial_TC {
public:
  //  class methods
  static Serial_TC *instance();

  void printf(const char *format, ...);
  void ts_printf(const char *format, ...);

private:
  //  class variable
  static Serial_TC *_instance;

  // instance variable
  bool printIsActive = false;

  //  instance methods
  Serial_TC();
};
