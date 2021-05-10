#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "SD_TC.h"
#include "TC_util.h"

unittest_setup() {
  SD_TC::instance()->removeAll();
}

unittest_teardown() {
  SD_TC::instance()->removeAll();
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
  SD_TC* sd = SD_TC::instance();
  state->serialPort[0].dataOut = "";
  assertEqual("", state->serialPort[0].dataOut);

  sd->open("c", FILE_WRITE).close();
  sd->open("e", FILE_WRITE).close();
  sd->mkdir("d");
  sd->open("d/d1", FILE_WRITE).close();
  sd->open("d/d2", FILE_WRITE).close();
  state->serialPort[0].dataOut = "";
  sd->printRootDirectory();
  String output = String(state->serialPort[0].dataOut);
  int index;
  index = output.indexOf("           c (     0)\r\n");
  assertNotEqual(-1, index);
  index = output.indexOf("           d/\r\n");
  assertNotEqual(-1, index);
  index = output.indexOf("-           d1 (     0)\r\n");
  assertNotEqual(-1, index);
  index = output.indexOf("-           d2 (     0)\r\n");
  assertNotEqual(-1, index);
  index = output.indexOf("           e (     0)\r\n");
  assertNotEqual(-1, index);
}

/*
           c (     0)
d/
-           d1 (     0)
-           d2 (     0)
           e (     0)
*/

unittest_main()
