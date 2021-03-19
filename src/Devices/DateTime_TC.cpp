#include "Devices/DateTime_TC.h"

#include "Devices/Serial_TC.h"

RTC_PCF8523 *DateTime_TC::_rtc = nullptr;

RTC_PCF8523 *DateTime_TC::rtc() {
  if (_rtc == nullptr) {
    _rtc = new RTC_PCF8523;
    // look for Real Time Clock
    if (!_rtc->begin()) {
      Serial.println(F("Couldn't find RTC!"));
      while (true)
        ;  // infinite loop; hang forever
    }

    // has the time been set?
    if (!_rtc->initialized()) {
      Serial.println(F("RTC has not been initialized! Date and time is wrong!"));
      // set the RTC to the date & time this file was compiled
      _rtc->adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  }
  return _rtc;
}

DateTime_TC DateTime_TC::now() {
  DateTime now = rtc()->now();
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
  char buffer[20];
  strcpy(buffer, "YYYY-MM-DD hh:mm:ss");
  toString(buffer);
  Serial_TC::instance()->print(buffer);
}

/**
 * populate a buffer with dateTime as a path: "/2020/09"
 */
void DateTime_TC::yearMonthAsPath(char *buffer, size_t sizeOfBuffer) {
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

void DateTime_TC::setAsCurrent() {
  DateTime newDT(yOff + 2000, m, d, hh, mm, ss);
  rtc()->adjust(newDT);
}
