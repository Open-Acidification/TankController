#pragma once
#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <RTClib_CI.h>
#else
#include <RTClib.h>
#endif

class DateTime_TC : public DateTime {
public:
  // class methods
  static DateTime_TC now();
  // instance methods
  DateTime_TC(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);
  void printToSerial();                             // DigitalClockDisplay.ino
  void yearMonthAsPath(char* buffer, size_t size);  // DoDirectoryMaintenance.ino
};
