#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DataLogger.h"
#include "DateTime_TC.h"
#include "MainMenu.h"
#include "PID_TC.h"
#include "SD_TC.h"
#include "Serial_TC.h"
#include "TankController.h"
#include "ThermalProbe_TC.h"
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
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
  // write to data log every second
  // write to serial log every minute
  // write to remote log every minute
  // write warning to remote log when needed

  assertEqual("", sd->mostRecentDataLogHeader);
  assertEqual("", sd->mostRecentDataLogLine);
  assertEqual("", serialPort->getBuffer());
  assertEqual("", sd->mostRecentRemoteLogEntry);

  // initial loop
  tc->loop();
  assertEqual("", sd->mostRecentDataLogHeader);
  assertEqual("", sd->mostRecentDataLogLine);
  assertEqual("heater turned on at 6 after 6 ms", serialPort->getBuffer());
  serialPort->clearBuffer();
  assertEqual("", sd->mostRecentRemoteLogEntry);

  // data log after one second
  delay(1000);
  tc->loop();  // write to data log
  tc->loop();  // should not write to serial log
  tc->loop();  // should not write data to remote log
  tc->loop();  // should not write warning to remote log
  assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,upTime,Kp,Ki,Kd", sd->mostRecentDataLogHeader);
  assertEqual("08/15/2023 00:00:01,   0, 0.00, 20.00, 0.000, 8.100,    1, 100000.0,      0.0,      0.0",
              sd->mostRecentDataLogLine);
  assertEqual("", serialPort->getBuffer());
  assertEqual("", sd->mostRecentRemoteLogEntry);

  // serial log after one minute
  delay(59000);
  tc->loop();  // write to data log
  tc->loop();  // write to serial log
  assertEqual("00:01 pH=0.000 temp= 0.00", serialPort->getBuffer());

  // remote log entry after one minute
  assertFalse(0.0 == thermalProbe->getSampleMean());
  tc->loop();                                                     // write info to remote log
  assertTrue(isnan(thermalProbe->getSampleMean()));               // thermal sample has been collected
  assertTrue(isnan(thermalProbe->getSampleStandardDeviation()));  // thermal sample has been reset
  char infoString[512] = "";
  snprintf(infoString, sizeof(infoString), "%s\t%s", VERSION,
           "0\tI\t2023-08-15 00:01:00\t\t20.00\t-242.02\t0.000\t8.100\t0.000\t60");
  assertEqual(infoString, sd->mostRecentRemoteLogEntry);
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
  tc->loop();  // write the warning
  char warningString[512] = "";
  snprintf(warningString, sizeof(warningString), "%s\t%s", VERSION,
           "0\tW\t2023-08-15 "
           "00:00:19\t\t\t\t\t\t\t19\t90:A2:DA:80:7B:76\tRequesting...\t1\t0.00\t1\t1\t0.00\t0.00\t100000.00\t0\t8."
           "10\t-1\t-1\t0.0\t-1\t-1\t0.0\t0\t20.00\t-1\t-1\t0.0\t-1\t-1\t0.0\t65535");
  assertEqual(warningString, sd->mostRecentRemoteLogEntry);
  // assertEqual("New warning written to log", serialPort->getBuffer());
  assertFalse(dl->getShouldWriteWarning());
}

unittest_main()
