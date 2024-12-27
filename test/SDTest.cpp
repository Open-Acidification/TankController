#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "PHCalibrationMid.h"
#include "RemoteLogPusher.h"
#include "SD_TC.h"
#include "TC_util.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->reset();
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

unittest(tankControllerLoop) {
  char data[250];
  TankController* tc = TankController::instance();
  DateTime_TC d1(2021, 4, 15);
  d1.setAsCurrent();
  assertFalse(SD_TC::instance()->exists("20210415.csv"));
  tc->loop(false);
  tc->loop(false);
  delay(1000);
  tc->loop(false);
  tc->loop(false);
  assertTrue(SD_TC::instance()->exists("20210415.csv"));
  File file = SD_TC::instance()->open("20210415.csv");
  assertTrue(file.size() < sizeof(data));
  if (file.size() < sizeof(data)) {
    file.read(data, file.size());
    data[file.size()] = '\0';
    auto expected =
        "time,tankid,temp,temp setpoint,pH,pH setpoint,upTime,Kp,Ki,Kd\n"
        "04/15/2021 00:00:00,   0, 0.00, 20.00, 0.000, 8.100,    1, 100000.0,      0.0,      0.0\n"
        "04/15/2021 00:00:01,   0, 0.00, 20.00, 0.000, 8.100,    2, 100000.0,      0.0,      0.0\n";
    assertEqual(expected, data);
  }
  file.close();
}

unittest(loopInCalibration) {
  TankController* tc = TankController::instance();
  PHCalibrationMid* test = new PHCalibrationMid();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  char data[250];
  DateTime_TC d1(2021, 4, 15);
  d1.setAsCurrent();
  assertFalse(SD_TC::instance()->exists("20210415.csv"));
  tc->loop(false);
  tc->loop(false);
  delay(3000);
  tc->loop(false);
  tc->loop(false);
  assertTrue(SD_TC::instance()->exists("20210415.csv"));
  File file = SD_TC::instance()->open("20210415.csv");
  assertTrue(file.size() < sizeof(data));
  if (file.size() < sizeof(data)) {
    file.read(data, file.size());
    data[file.size()] = '\0';
    auto expected =
        "time,tankid,temp,temp setpoint,pH,pH setpoint,upTime,Kp,Ki,Kd\n"
        "04/15/2021 00:00:03,   0, C, 20.00, C, 8.100,    3, 100000.0,      0.0,      0.0\n";
    assertEqual(expected, data);
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
  sd->writeToDataLog("time,tankid,temp,temp setpoint,pH,pH setpoint,upTime,Kp,Ki,Kd", "line 1");
  sd->writeToDataLog("time,tankid,temp,temp setpoint,pH,pH setpoint,upTime,Kp,Ki,Kd", "line 2");
  assertTrue(SD_TC::instance()->exists("20210415.csv"));
  assertFalse(SD_TC::instance()->exists("20210416.csv"));

  // write data for day 16
  d2.setAsCurrent();
  sd->writeToDataLog("time,tankid,temp,temp setpoint,pH,pH setpoint,upTime,Kp,Ki,Kd", "line 3");
  assertTrue(SD_TC::instance()->exists("20210415.csv"));
  assertTrue(SD_TC::instance()->exists("20210416.csv"));

  // verify contents of 15.csv
  File file = SD_TC::instance()->open("20210415.csv");
  file.read(data, file.size());
  data[file.size()] = '\0';
  auto expected1 = "time,tankid,temp,temp setpoint,pH,pH setpoint,upTime,Kp,Ki,Kd\nline 1\nline 2\n";
  assertEqual(expected1, data);
  file.close();

  // verify contents of 16.csv
  file = SD_TC::instance()->open("20210416.csv");
  file.read(data, file.size());
  data[file.size()] = '\0';
  auto expected2 = "time,tankid,temp,temp setpoint,pH,pH setpoint,upTime,Kp,Ki,Kd\nline 3\n";
  assertEqual(expected2, data);
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

unittest(removeFile) {
  DateTime_TC date(2022, 7, 6);
  SD_TC* sd = SD_TC::instance();
  assertFalse(SD_TC::instance()->exists("20220706.log"));
  date.setAsCurrent();
  sd->appendToLog("contents for file");
  assertTrue(SD_TC::instance()->exists("20220706.log"));
  sd->remove("20220706.log");
  assertFalse(SD_TC::instance()->exists("20220706.log"));
}

unittest(writeRemoteLog) {
  delay(60000);  // remote logs don't get written immediately
  char data[20];
  SD_TC* sd = SD_TC::instance();
  RemoteLogPusher* pusher = RemoteLogPusher::instance();

  assertEqual("90A2DA807B76.log", sd->getRemoteLogName());
  // because sd was previously initialized, we have remoteLogFileNameIsReady == true
  sd->updateRemoteLogFileSizeForTest();
  assertFalse(sd->exists("90A2DA807B76.log"));
  assertEqual(0, sd->getRemoteFileSize());
  pusher->setShouldSentHeadRequest(false);
  assertFalse(pusher->shouldSendHeadRequest());

  // write data
  sd->writeToRemoteLog("line 1");  // also writes header row
  sd->updateRemoteLogFileSizeForTest();
  assertTrue(pusher->basicShouldSendHeadRequest());
  assertTrue(sd->exists("90A2DA807B76.log"));
  int size = sd->getRemoteFileSize();
  sd->writeToRemoteLog("line 2");
  sd->updateRemoteLogFileSizeForTest();
  assertEqual(size + strlen("line 2\n"), sd->getRemoteFileSize());

  // verify contents of remote log
  File file = sd->open("90A2DA807B76.log");
  file.seek(size);
  file.read(data, 7);
  file.close();
  data[7] = '\0';
  assertEqual("line 2\n", data);
}

unittest(getRemoteLogContents) {
  SD_TC* sd = SD_TC::instance();

  // write data
  sd->setRemoteLogName("Tank1");
  sd->writeToRemoteLog("line 1");
  sd->updateRemoteLogFileSizeForTest();
  int size = sd->getRemoteFileSize();
  sd->writeToRemoteLog("and 2\nline 3");
  sd->updateRemoteLogFileSizeForTest();
  char buffer[20];
  // get remaining remote log
  sd->getRemoteLogContents(buffer, sizeof(buffer), size);
  assertEqual("and 2\nline 3\n", buffer);
}

unittest(noRemoteLogFileName) {
  SD_TC* sd = SD_TC::instance();
  sd->setRemoteLogName("");
  assertEqual("90A2DA807B76.log", sd->getRemoteLogName());
}

unittest(validRemoteLogFileName) {
  SD_TC* sd = SD_TC::instance();
  sd->setRemoteLogName("Tank1");
  assertEqual("Tank1.log", sd->getRemoteLogName());
}

unittest(longRemoteLogFileName) {
  SD_TC* sd = SD_TC::instance();
  sd->setRemoteLogName("1234567890123456789012345678");  // maximum length
  assertEqual("1234567890123456789012345678.log", sd->getRemoteLogName());
  sd->setRemoteLogName("12345678901234567890123456789");  // one character too many
  assertEqual("90A2DA807B76.log", sd->getRemoteLogName());
}

unittest(remoteLogName) {
  TankController::deleteInstance();
  TankController* tc = TankController::instance("remoteLog");
  SD_TC* sd = SD_TC::instance();
  const char* name = sd->getRemoteLogName();
  assertEqual("remoteLog.log", name);

  TankController::deleteInstance();
  SD_TC::deleteInstance();
  tc = TankController::instance();
  sd = SD_TC::instance();
  name = sd->getRemoteLogName();
  assertEqual("90A2DA807B76.log", name);

  sd->setRemoteLogName("newName");
  name = sd->getRemoteLogName();
  assertEqual("newName.log", name);
}

unittest_main()
