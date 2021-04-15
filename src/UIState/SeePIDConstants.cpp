/**
 * SeePIDConstants.cpp
 */

#include "SeePIDConstants.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "Serial_TC.h"
#include "TC_util.h"

void SeePIDConstants::loop() {
  // Precheck
  int elapsedSeconds = (millis() - startTime) / 1000;

  Serial_TC *mySerial = Serial_TC::instance();
  mySerial->print("SeePIDConstants::loop() - 1 CurrentTime ", elapsedSeconds);
  switch (elapsedSeconds) {
    case 0:
      loadKp(0);
      loadKi(1);
      break;
    case 1:
      loadKi(0);
      loadKd(1);
      break;
    case 2:
      loadKd(0);
      loadSlope(1);
      break;
    case 3:
      loadSlope(0);
      loadKp(1);
      break;
    case 4:
      loadKp(0);
      loadKi(1);
      break;
  }
  if (elapsedSeconds >= 5) {
    COUT("done");
    returnToMainMenu();
  }
}

void SeePIDConstants::loadKp(int line) {
  PID *pPID = PID_TC::instance()->getPID();
  char buffer[17];
  sprintf(buffer, "Kp: %.1f", pPID->GetKp());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKi(int line) {
  PID *pPID = PID_TC::instance()->getPID();
  char buffer[17];
  sprintf(buffer, "Ki: %.1f", pPID->GetKi());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKd(int line) {
  PID *pPID = PID_TC::instance()->getPID();
  char buffer[17];
  sprintf(buffer, "Kd: %.1f", pPID->GetKd());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadSlope(int line) {
  char buffer[17];
  sprintf(buffer, "Slope:%.1f", PHProbe::instance()->getSlope());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}
