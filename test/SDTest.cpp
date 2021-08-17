#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "SD_TC.h"
#include "TC_util.h"
#include "TankControllerLib.h"
#include "UIState/PHCalibrationMid.h"

unittest_setup() {
  SD_TC::instance()->format();
}

unittest_teardown() {
  SD_TC::instance()->format();
}

unittest(singleton) {
  SD_TC* thing1 = SD_TC::instance();
  SD_TC* thing2 = SD_TC::instance();
  assertTrue(thing1 != nullptr);
  assertEqual(thing1, thing2);
}

/*

unittest(tankControllerLoop) {
  char data[250];
  TankControllerLib* tc = TankControllerLib::instance();
  DateTime_TC d1(2021, 4, 15);
  d1.setAsCurrent();
  assertFalse(SD_TC::instance()->exists("20210415.csv"));
  tc->loop();
  delay(1000);
  tc->loop();
  assertTrue(SD_TC::instance()->exists("20210415.csv"));
  File file = SD_TC::instance()->open("20210415.csv");
  assertTrue(file.size() < sizeof(data));
  if (file.size() < sizeof(data)) {
    file.read(data, file.size());
    data[file.size()] = '\0';
    assertEqual(
        "time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd\n"
        "04/15/2021 00:00:00,   0, -242.02, 20.00, 0.000, 8.100,    1, 100000.0,      0.0,      0.0\n"
        "04/15/2021 00:00:01,   0, -242.02, 20.00, 0.000, 8.100,    2, 100000.0,      0.0,      0.0\n",
        data);
  }
  file.close();
}

unittest(loopInCalibration) {
  TankControllerLib* tc = TankControllerLib::instance();
  PHCalibrationMid* test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  char data[250];
  DateTime_TC d1(2021, 4, 15);
  d1.setAsCurrent();
  assertFalse(SD_TC::instance()->exists("20210415.csv"));
  tc->loop();
  delay(1000);
  tc->loop();
  assertTrue(SD_TC::instance()->exists("20210415.csv"));
  File file = SD_TC::instance()->open("20210415.csv");
  assertTrue(file.size() < sizeof(data));
  if (file.size() < sizeof(data)) {
    file.read(data, file.size());
    data[file.size()] = '\0';
    assertEqual(
        "time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd\n"
        "04/15/2021 00:00:01,   0, C, 20.00, C, 8.100,    3, 100000.0,      0.0,      0.0\n",
        data);
  }
  file.close();
}

unittest(appendData) {
  char data[80];
  DateTime_TC d1(2021, 4, 15), d2(2021, 4, 16);
  SD_TC* sd = SD_TC::instance();

  assertFalse(SD_TC::instance()->exists("20210415.csv"));
  assertFalse(SD_TC::instance()->exists("20210416.csv"));

  // write data for day 15
  d1.setAsCurrent();
  sd->appendData("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", "line 1");
  sd->appendData("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", "line 2");
  assertTrue(SD_TC::instance()->exists("20210415.csv"));
  assertFalse(SD_TC::instance()->exists("20210416.csv"));

  // write data for day 16
  d2.setAsCurrent();
  sd->appendData("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", "line 3");
  assertTrue(SD_TC::instance()->exists("20210415.csv"));
  assertTrue(SD_TC::instance()->exists("20210416.csv"));

  // verify contents of 15.csv
  File file = SD_TC::instance()->open("20210415.csv");
  file.read(data, file.size());
  data[file.size()] = '\0';
  assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd\nline 1\nline 2\n", data);
  file.close();

  // verify contents of 16.csv
  file = SD_TC::instance()->open("20210416.csv");
  file.read(data, file.size());
  data[file.size()] = '\0';
  assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd\nline 3\n", data);
  file.close();
}

unittest(appendToLog) {
  char data[80];
  DateTime_TC d1(2021, 4, 15), d2(2021, 4, 16);
  SD_TC* sd = SD_TC::instance();

  assertFalse(SD_TC::instance()->exists("20210415.log"));
  assertFalse(SD_TC::instance()->exists("20210416.log"));

  // write data for day 15
  d1.setAsCurrent();
  sd->appendToLog("line 1");
  sd->appendToLog("line 2");
  assertTrue(SD_TC::instance()->exists("20210415.log"));
  assertFalse(SD_TC::instance()->exists("20210416.log"));

  // write data for day 16
  d2.setAsCurrent();
  sd->appendToLog("line 3");
  assertTrue(SD_TC::instance()->exists("20210415.log"));
  assertTrue(SD_TC::instance()->exists("20210416.log"));

  // verify contents of 15.log
  File file = SD_TC::instance()->open("20210415.log");
  file.read(data, file.size());
  data[file.size()] = '\0';
  assertEqual("line 1\nline 2\n", data);
  file.close();

  // verify contents of 16.log
  file = SD_TC::instance()->open("20210416.log");
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

  SD_TC::instance()->open("c", O_WRONLY).close();
  SD_TC::instance()->open("e", O_WRONLY).close();
  SD_TC::instance()->mkdir("d");
  SD_TC::instance()->open("d/d1", O_WRONLY).close();
  SD_TC::instance()->open("d/d2", O_WRONLY).close();
  state->serialPort[0].dataOut = "";
  sd->printRootDirectory();
  String output = String(state->serialPort[0].dataOut);
  uint16_t index;
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
*/
unittest_main()
