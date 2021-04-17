#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "SD_TC.h"

unittest_setup() {
  SD.removeAll();
}

unittest_teardown() {
  SD.removeAll();
}

unittest(singleton) {
  SDClass_TC* thing1 = SDClass_TC::instance();
  SDClass_TC* thing2 = SDClass_TC::instance();
  assertTrue(thing1 != nullptr);
  assertEqual(thing1, thing2);
}

unittest(appendToSerialLog) {
  char data[20];
  DateTime_TC d1(2021, 4, 15), d2(2021, 4, 16);
  SDClass_TC* sd = SDClass_TC::instance();
  // char buffer[9];
  // date.yearMonthAsPath(buffer, sizeof(buffer));
  // assertEqual("/2020/09", buffer);

  assertFalse(sd->exists("/log/2021/04/15.txt"));
  assertFalse(sd->exists("/log/2021/04/16.txt"));

  // write data for day 15
  d1.setAsCurrent();
  sd->appendToSerialLog("line 1");
  sd->appendToSerialLog("line 2");
  assertTrue(sd->exists("/log/2021/04/15.txt"));
  assertFalse(sd->exists("/log/2021/04/16.txt"));

  // write data for day 16
  d2.setAsCurrent();
  sd->appendToSerialLog("line 3");
  assertTrue(sd->exists("/log/2021/04/15.txt"));
  assertTrue(sd->exists("/log/2021/04/15.txt"));

  // verify contents of 15.txt
  File file = sd->open("/log/2021/04/15.txt");
  file.read(data, file.size());
  assertTrue(strcmp(data, "line 1\nline 2\n") == 0);
  file.close();

  // verify contents of 16.txt
  file = sd->open("/log/2021/04/16.txt");
  file.read(data, file.size());
  assertTrue(strcmp(data, "line 3\n") == 0);
  file.close();
}

unittest(printRootDirectory) {
  GodmodeState* state = GODMODE();

  SD.open("c", FILE_WRITE).close();
  SD.open("e", FILE_WRITE).close();
  SD.mkdir("d");
  SD.open("d/d1", FILE_WRITE).close();
  SD.open("d/d2", FILE_WRITE).close();

  SDClass_TC::instance()->printRootDirectory();
  String expect = String("c>>0\r\nd/\r\n>d1>>0\r\n>d2>>0\r\ne>>0\r\n");
  String output = String(state->serialPort[0].dataOut);
  std::replace(output.begin(), output.end(), '\t', '>');
  assertEqual(expect, output);
}

unittest_main()
