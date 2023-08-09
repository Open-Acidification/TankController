#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DataLogger.h"
#include "MainMenu.h"
#include "SD_TC.h"
#include "Serial_TC.h"
#include "TankController.h"

TankController* tc = TankController::instance();
Serial_TC* serialPort = Serial_TC::instance();
SD_TC* sd = SD_TC::instance();

unittest_setup() {
  GODMODE()->resetClock();
  tc->setNextState(new MainMenu(), true);
  DataLogger::instance()->clearBuffer();
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
  assertEqual("08/08/2023 16:47:13,   0, 0.00, 20.00, 0.000, 8.100,   59, 100000.0,      0.0,      0.0",
              sd->mostRecentLine);
  assertEqual("", serialPort->getBuffer());
  tc->loop(false);  // write to serial
  assertEqual("16:47 pH=0.000 temp= 0.00", serialPort->getBuffer());
  delay(1000);
  tc->loop(false);
  assertEqual("New info written to log", serialPort->getBuffer());
}

unittest(writeToSDTest) {
  // TODO: This is tested in SDTest.cpp but should be moved here so that those tests are not dependent on DataLogger's
  // loop structure
}

unittest(writeToSerialTest) {
  // TODO
}

unittest_main()
