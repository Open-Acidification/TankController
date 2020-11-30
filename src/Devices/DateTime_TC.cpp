#include "Devices/DateTime_TC.h"

#include "Devices/Serial_TC.h"

//  class methods
DateTime_TC DateTime_TC::now() {
  DateTime now = RTC_PCF8523::now();
  return DateTime_TC(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
}

//  instance methods
/**
 * Constructor
 */
DateTime_TC::DateTime_TC(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
    : DateTime(year, month, day, hour, min, sec) {
}

/**
 * output dateTime to serialPort(DigitalClockDisplay.ino)
 * "2020-11-26 18:55:15"
 */
void DateTime_TC::printToSerial() {
  Serial_TC* serial = Serial_TC::instance();
  serial->print(year());
  serial->print('-');
  serial->print_two_digits(month());
  serial->print('-');
  serial->print_two_digits(day());
  serial->print(' ');
  serial->print_two_digits(hour());
  serial->print(':');
  serial->print_two_digits(minute());
  serial->print(':');
  serial->print_two_digits(second());
  serial->println();
}

/**
 * populate a buffer with dateTime as a path: "/2020/09"
 */
void DateTime_TC::yearMonthAsPath(char* buffer, size_t sizeOfBuffer) {
  memset(buffer, 0, sizeOfBuffer);
  if (sizeOfBuffer < 9) {  // is there enough room?
    return;
  }
  buffer[0] = '/';
  itoa(year(), buffer + 1, 10);
  buffer[5] = '/';
  int i = 6;
  if (month() < 10) {
    buffer[i++] = '0';
  }
  itoa(month(), buffer + i, 10);
}
