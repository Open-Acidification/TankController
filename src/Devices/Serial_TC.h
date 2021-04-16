/**
 * This class is a wrapper for Arduino's Serial interface.
 * The only function it supports is printf() and Serial::println()
 * will add a CRLF to the end of each line.
 */
#pragma once

#include <Arduino.h>

class Serial_TC {
public:
  //  class methods
  static Serial_TC *instance();

  void printf(const char *format, ...);

private:
  //  class variable
  static Serial_TC *_instance;

  //  instance methods
  Serial_TC();
};
