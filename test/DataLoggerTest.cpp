#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DataLogger.h"
#include "DateTime_TC.h"
#include "MainMenu.h"
#include "SD_TC.h"
#include "Serial_TC.h"
#include "TankController.h"
#include "Version.h"

TankController* tc = TankController::instance();
Serial_TC* serialPort = Serial_TC::instance();
SD_TC* sd = SD_TC::instance();
DataLogger* dl = DataLogger::instance();

unittest_setup() {
  GODMODE()->resetClock();
  DateTime_TC d1(2023, 8, 15);
  d1.setAsCurrent();
  tc->setNextState(new MainMenu(), true);
  dl->reset();
  serialPort->clearBuffer();
}

unittest(loop) {
  tc->loop(false);
  delay(59000);
  serialPort->clearBuffer();
  assertEqual("", sd->mostRecentHeader);
  assertEqual("", sd->mostRecentLine);
  tc->loop(false);  // write to SD card
  assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", sd->mostRecentHeader);
  assertEqual("08/15/2023 00:00:59,   0, 0.00, 20.00, 0.000, 8.100,   59, 100000.0,      0.0,      0.0",
              sd->mostRecentLine);
  assertEqual("", serialPort->getBuffer());
  tc->loop(false);  // write to serial
  assertEqual("00:00 pH=0.000 temp= 0.00", serialPort->getBuffer());
  delay(1000);
  tc->loop(false);  // write info to log file
  char infoString[512] = "";
  snprintf(infoString, sizeof(infoString), "%s\t%s", VERSION, "0\tI\t2023-08-15 00:01:00\t0.00\t20.00\t0.000\t8.100");
  assertEqual(infoString, sd->mostRecentStatusEntry);
  assertEqual("New info written to log", serialPort->getBuffer());
}

unittest(writeToSD) {
  // TODO: This is tested in SDTest.cpp but should be moved here so that those tests are not dependent on DataLogger's
  // loop structure
}

unittest(writeToSerial) {
  // TODO
}

unittest(writeWarningToLog) {
  assertFalse(dl->getShouldWriteWarning());
  dl->writeWarningSoon();
  assertTrue(dl->getShouldWriteWarning());
  delay(19000);
  tc->loop(false);  // write the warning
  char warningString[512] = "";
  snprintf(warningString, sizeof(warningString), "%s\t%s", VERSION,
           "0\tW\t2023-08-15 "
           "00:00:19\t19\t90:A2:DA:80:7B:76\tRequesting...\t1\t0.00\t1\t1\t0.00\t0.00\t100000.00\t0\t8.10\t-1\t-1\t0."
           "00\t-1\t-1\t0.00\t0\t20.00\t-1\t-1\t0.00\t-1\t-1\t0.00\t65535");
  assertEqual(warningString, sd->mostRecentStatusEntry);
  // assertEqual("New warning written to log", serialPort->getBuffer());
  assertFalse(dl->getShouldWriteWarning());
}

unittest_main()
