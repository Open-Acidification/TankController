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

int SeePIDConstants::writeToBuffer(char *buffer, int size, int line) {
  // Line in this case is not an LCD line since this is handled by 4 load functions
  // 0 = Kp, 1 = Ki, 2 = Kd, 3 = PID
  float value;
  switch (line) {
    case 0:
      value = PID_TC::instance()->getKp();
      if (size > 17) {
        return snprintf_P(buffer, size, (PGM_P)F("\"Kp\":%i.%i"), (int)value, (int)(value * 10 + 0.5) % 10);
      } else {
        return snprintf_P(buffer, size, (PGM_P)F("Kp: %i.%i"), (int)value, (int)(value * 10 + 0.5) % 10);
      }
      break;
    case 1:
      value = PID_TC::instance()->getKi();
      if (size > 17) {
        return snprintf_P(buffer, size, (PGM_P)F("\"Ki\":%i.%i"), (int)value, (int)(value * 10 + 0.5) % 10);
      } else {
        return snprintf_P(buffer, size, (PGM_P)F("Ki: %i.%i"), (int)value, (int)(value * 10 + 0.5) % 10);
      }
      break;
    case 2:
      value = PID_TC::instance()->getKd();
      if (size > 17) {
        return snprintf_P(buffer, size, (PGM_P)F("\"Kd\":%i.%i"), (int)value, (int)(value * 10 + 0.5) % 10);
      } else {
        return snprintf_P(buffer, size, (PGM_P)F("Kd: %i.%i"), (int)value, (int)(value * 10 + 0.5) % 10);
      }
      break;
    case 3:
      if (PHControl::instance()->getUsePID()) {
        if (size > 17) {
          return snprintf_P(buffer, size, (PGM_P)F("\"PID\":\"ON\""));
        } else {
          return snprintf_P(buffer, size, (PGM_P)F("PID: ON"));
        }
      } else {
        if (size > 17) {
          return snprintf_P(buffer, size, (PGM_P)F("\"PID\": \"OFF\""));
        } else {
          return snprintf_P(buffer, size, (PGM_P)F("PID: OFF"));
        }
      }
      break;
    default:
      break;  // TODO: Add error handling?
  }
}

void SeePIDConstants::loadKp(uint16_t line) {
  char buffer[17];
  writeToBuffer(buffer, sizeof(buffer), 0);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKi(uint16_t line) {
  char buffer[17];
  writeToBuffer(buffer, sizeof(buffer), 1);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadKd(uint16_t line) {
  char buffer[17];
  writeToBuffer(buffer, sizeof(buffer), 2);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePIDConstants::loadPID(uint16_t line) {
  char buffer[9];  // Longest value is "PID: OFF\0"
  writeToBuffer(buffer, sizeof(buffer), 3);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}
