#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"

/**
 * year has to be range: 2000-2099
 * following command only runs this test
 * bundle exec arduino_ci.rb --skip-examples-compilation
 * --testfile-select=DateTime.cpp
 */

unittest(singleton) {
  DateTime_TC dt(2021, 2, 15, 1, 48, 24);
  RTC_PCF8523 *rtc1 = dt.rtc();
  RTC_PCF8523 *rtc2 = dt.rtc();
  assertEqual(rtc1, rtc2);
}

unittest(constructor) {
  DateTime_TC dt(2021, 2, 15, 1, 48, 24);
  assertEqual(2021, dt.year());
  assertEqual(2, dt.month());
  assertEqual(15, dt.day());
  assertEqual(1, dt.hour());
  assertEqual(48, dt.minute());
  assertEqual(24, dt.second());
}

unittest(now) {
  DateTime_TC now = DateTime_TC::now();
  assertTrue(now.year() >= 2020);
  assertTrue(now.month() >= 1);
  assertTrue(now.day() >= 1);
  assertTrue(now.hour() >= 0);
  assertTrue(now.minute() >= 0);
  assertTrue(now.second() >= 0);
}

unittest(as16CharacterString) {
  DateTime_TC dateTime = DateTime_TC(2021, 03, 19, 15, 50);
  dateTime.printToSerial();
  assertEqual("2021-03-19 15:50", dateTime.as16CharacterString());
}

//  This function came from DigitalClockDisplay.ino in old TankController code
unittest(printToSerial) {
  GodmodeState *state = GODMODE();
  state->reset();
  assertEqual("", state->serialPort[0].dataOut);
  DateTime_TC dateTime = DateTime_TC(2020, 11, 26, 18, 55, 15);
  dateTime.printToSerial();
  assertEqual("2020-11-26 18:55:15\r\n", state->serialPort[0].dataOut);
}

// DoDirectoryMaintenance.ino
unittest(yearMonthAsPath) {
  DateTime_TC date = DateTime_TC(2020, 9, 26);
  char buffer[9];
  date.yearMonthAsPath(buffer, sizeof(buffer));
  assertEqual("/2020/09", buffer);
}

unittest(setAsCurrent) {
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();

  DateTime_TC now1 = DateTime_TC::now();
  assertEqual(2021, now1.year());
  assertEqual(1, now1.month());
  assertEqual(15, now1.day());
  assertEqual(1, now1.hour());
  assertEqual(48, now1.minute());
  assertEqual(24, now1.second());
}

// Remaining test deals with simulated delays
#undef yield
#include <chrono>
#include <thread>

#include "UIState/UIState.h"

unittest(myDelay) {
  uint32_t t1 = DateTime_TC::now().unixtime();

  auto start = std::chrono::high_resolution_clock::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  delay(10000);  // this should be seen by arduino_ci, but not the underlying system
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  // actual delay should a few milliseconds
  assertTrue(1 <= elapsed.count() && elapsed.count() <= 5);

  uint32_t t2 = DateTime_TC::now().unixtime();
  // simulated delay should be 10 seconds
  assertTrue(t1 + 10 <= t2 && t2 <= t1 + 11);
}

unittest_main()
