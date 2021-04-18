#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "SD_TC.h"
#include "TC_util.h"

unittest_setup() {
  SD.removeAll();
}

unittest_teardown() {
  SD.removeAll();
}

unittest(singleton) {
  SD_TC* thing1 = SD_TC::instance();
  SD_TC* thing2 = SD_TC::instance();
  assertTrue(thing1 != nullptr);
  assertEqual(thing1, thing2);
}

unittest(appendToDataLog) {
  char data[80];
  DateTime_TC d1(2021, 4, 15), d2(2021, 4, 16);
  SD_TC* sd = SD_TC::instance();

  assertFalse(sd->exists("/data/2021/04/15.txt"));
  assertFalse(sd->exists("/data/2021/04/16.txt"));

  // write data for day 15
  d1.setAsCurrent();
  sd->appendToDataLog("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", "line 1");
  sd->appendToDataLog("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", "line 2");
  assertTrue(sd->exists("/data/2021/04/15.txt"));
  assertFalse(sd->exists("/data/2021/04/16.txt"));

  // write data for day 16
  d2.setAsCurrent();
  sd->appendToDataLog("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", "line 3");
  assertTrue(sd->exists("/data/2021/04/15.txt"));
  assertTrue(sd->exists("/data/2021/04/16.txt"));

  // verify contents of 15.txt
  File file = sd->open("/data/2021/04/15.txt");
  file.read(data, file.size());
  data[file.size()] = '\0';
  assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd\nline 1\nline 2\n", data);
  file.close();

  // verify contents of 16.txt
  file = sd->open("/data/2021/04/16.txt");
  file.read(data, file.size());
  data[file.size()] = '\0';
  assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd\nline 3\n", data);
  file.close();
}

unittest(appendToSerialLog) {
  char data[80];
  DateTime_TC d1(2021, 4, 15), d2(2021, 4, 16);
  SD_TC* sd = SD_TC::instance();

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
  assertTrue(sd->exists("/log/2021/04/16.txt"));

  // verify contents of 15.txt
  File file = sd->open("/log/2021/04/15.txt");
  file.read(data, file.size());
  data[file.size()] = '\0';
  assertEqual("line 1\nline 2\n", data);
  file.close();

  // verify contents of 16.txt
  file = sd->open("/log/2021/04/16.txt");
  file.read(data, file.size());
  data[file.size()] = '\0';
  assertEqual("line 3\n", data);
  file.close();
}

unittest(printRootDirectory) {
  GodmodeState* state = GODMODE();

  SD.open("c", FILE_WRITE).close();
  SD.open("e", FILE_WRITE).close();
  SD.mkdir("d");
  SD.open("d/d1", FILE_WRITE).close();
  SD.open("d/d2", FILE_WRITE).close();
  state->serialPort[0].dataOut = "";
  SDClass_TC::instance()->printRootDirectory();
  String expect = String("c>>0\r\nd/\r\n>d1>>0\r\n>d2>>0\r\ne>>0\r\n");
  String output = String(state->serialPort[0].dataOut);
  std::replace(output.begin(), output.end(), '\t', '>');
  assertEqual(expect, output);
}

unittest_main()
