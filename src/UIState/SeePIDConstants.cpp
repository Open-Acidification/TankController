/**
 * SeePIDConstants.cpp
 */

#include "SeePIDConstants.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"

void SeePIDConstants::loop() {
  int elapsedSeconds = ((millis() - startTime) / 3000) % 2;
  switch (elapsedSeconds) {
    case 0:
      loadKp(0);
      loadKi(1);
      break;
    case 1:
      loadKd(0);
      LiquidCrystal_TC::instance()->writeLine("               ", 1);
      break;
  }
}

void SeePIDConstants::loadKp(int line) {
  char buffer[17];
  snprintf(buffer, sizeof(buffer), "Kp: %5i.%1i", FLOAT(PID_TC::instance()->getKp(), 1));
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKi(int line) {
  char buffer[17];
  snprintf(buffer, sizeof(buffer), "Ki: %5i.%1i", FLOAT(PID_TC::instance()->getKi(), 1));
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKd(int line) {
  char buffer[17];
  snprintf(buffer, sizeof(buffer), "Kd: %5i.%1i", FLOAT(PID_TC::instance()->getKd(), 1));
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}
