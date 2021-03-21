#pragma once
#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <RTClib_CI.h>
#else
#include <RTClib.h>
#endif
/**
 * year range: 2000 - 2099
 */
class DateTime_TC : public DateTime {
public:
  // Constructors
  DateTime_TC(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);

  // class methods
  static DateTime_TC now();
  static RTC_PCF8523 *rtc();  // public so we can test

  // instance methods
  char *as16CharacterString();
  void printToSerial();                             // DigitalClockDisplay.ino
  void yearMonthAsPath(char *buffer, size_t size);  // DoDirectoryMaintenance.ino
  void setAsCurrent();

private:
  static RTC_PCF8523 *_rtc;
};
