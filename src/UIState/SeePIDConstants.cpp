/**
 * SeePIDConstants.cpp
 */

#include "SeePIDConstants.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "TC_util.h"

void SeePIDConstants::loop() {
  // Precheck
  if ((msEnd - millis()) <= (1000 * secondsLeft)) {
    char buffer[100];
    sprintf(buffer, "Rotate Display Time=%i", msEnd - millis());
    COUT(buffer);
    secondsLeft--;
  }

  switch (secondsLeft) {
    case 4:
      loadKp(0);
      loadKi(1);
      break;
    case 3:
      loadKi(0);
      loadKd(1);
      break;
    case 2:
      loadKd(0);
      loadSlope(1);
      break;
    case 1:
      loadSlope(0);
      loadKp(1);
      break;
    case 0:
      loadKp(0);
      loadKi(1);
      break;
  }

  if (msEnd < millis()) {
    COUT("done");
    returnToMainMenu();
  }
}

void SeePIDConstants::loadKp(int line) {
  PID *pPID = PID_TC::instance()->getPID();
  char buffer[17];
  sprintf(buffer, "kp:%.1f", pPID->GetKp());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKi(int line) {
  PID *pPID = PID_TC::instance()->getPID();
  char buffer[17];
  sprintf(buffer, "ki:%.1f", pPID->GetKi());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKd(int line) {
  PID *pPID = PID_TC::instance()->getPID();
  char buffer[17];
  sprintf(buffer, "kd:%.1f", pPID->GetKd());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadSlope(int line) {
  char buffer[17];
  sprintf(buffer, "Slope:%.1f", PHProbe::instance()->getSlope());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}
