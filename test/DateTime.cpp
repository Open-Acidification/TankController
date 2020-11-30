#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "TankControllerLib.h"

/*
bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=DateTime.cpp
*/

unittest(constructor) {
  DateTime_TC now = DateTime_TC::now();
  assertTrue(now.year() >= 2020);
}

//  void printToSerial();   // DigitalClockDisplay.ino
unittest(printToSerial) {
  GodmodeState* state = GODMODE();
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

unittest_main()
