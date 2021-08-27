/**
 * SeePIDConstants.cpp
 */

#include "SeePIDConstants.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "TC_util.h"

void SeePIDConstants::loop() {
  int32_t elapsedSeconds = ((millis() - startTime) / 3000) % 2;
  switch (elapsedSeconds) {
    case 0:
      loadKp(0);
      loadKi(1);
      break;
    case 1:
      loadKd(0);
      loadPID(1);
      break;
  }
}

void SeePIDConstants::loadKp(uint16_t line) {
  char buffer[17];
  snprintf(buffer, sizeof(buffer), "Kp: %.1f", PID_TC::instance()->getKp());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKi(uint16_t line) {
  char buffer[17];
  snprintf(buffer, sizeof(buffer), "Ki: %.1f", PID_TC::instance()->getKi());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKd(uint16_t line) {
  char buffer[17];
  snprintf(buffer, sizeof(buffer), "Kd: %.1f", PID_TC::instance()->getKd());
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadPID(uint16_t line) {
  if (PHControl::instance()->getUsePID()) {
    LiquidCrystal_TC::instance()->writeLine("PID: ON", line);
  } else {
    LiquidCrystal_TC::instance()->writeLine("PID: OFF", line);
  }
}
