/**
 * SeePIDConstants.cpp
 */

#include "SeePh.h"

#include "Devices/DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "TC_util.h"

void SeePh::loop() {
  int32_t elapsedSeconds = ((millis() - startTime) / 3000) % 2;
  switch (elapsedSeconds) {
    case 0:
      loadTarget(0);
      loadCurrent(1);
      break;
    case 1:
      loadPhSetType(0);
      loadTypeVariables(1);
      break;
  }
}

void SeePh::loadPhSetType(uint16_t line) {
  char buffer[17];
  // int value = PHControl::instance()->getPhSetType();
  char *type;
  switch (PHControl::instance()->getPhSetType()) {
    case FLAT_TYPE: {
      type = (char *)"flat";
      break;
    }
    case RAMP_TYPE: {
      type = (char *)"ramp";
      break;
    }
    case SINE_TYPE: {
      type = (char *)"sine";
      break;
    }
    default:
      break;
  }
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("type: %s"), type);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePh::loadTypeVariables(uint16_t line) {
  char buffer[17];
  switch (PHControl::instance()->getPhSetType()) {
    case FLAT_TYPE: {
      break;
    }
    case RAMP_TYPE: {
      uint32_t endTime = PHControl::instance()->getPhRampTimeEnd();
      uint32_t currentTime = DateTime_TC::now().secondstime();
      float timeLeft = endTime - currentTime;
      float timeLeftMinutes = timeLeft / 60;
      float timeLeftHours = timeLeft / 3600;
      int timeLeftSeconds = (int)timeLeft % 60;
      if (timeLeft > 0) {
        snprintf_P(buffer, sizeof(buffer), (PGM_P)F("left: %i:%i:%i"), (int)timeLeftHours, (int)timeLeftMinutes,
                   (int)timeLeftSeconds);
        LiquidCrystal_TC::instance()->writeLine(buffer, line);
      } else {
        snprintf_P(buffer, sizeof(buffer), (PGM_P)F("left: %i:%i:%i"), 0, 0, 0);
        LiquidCrystal_TC::instance()->writeLine(buffer, line);
      }
      break;
    }
    case SINE_TYPE: {
      float period = PHControl::instance()->getPeriod();
      float periodHours = period / 3600;
      float amplitude = PHControl::instance()->getAmplitude();

      snprintf_P(buffer, sizeof(buffer), (PGM_P)F("p=%i.%i a=%i.%i"), (int)periodHours,
                 (int)(periodHours * 1000) % 1000, (int)amplitude, (int)(amplitude * 1000) % 1000);
      LiquidCrystal_TC::instance()->writeLine(buffer, line);
      break;
    }
    default:
      break;
  }
}

void SeePh::loadTarget(uint16_t line) {
  char buffer[17];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Now Target Goal"));
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePh::loadCurrent(uint16_t line) {
  char buffer[17];
  float overallTargetPh = PHControl::instance()->getTargetPh();
  float currentTargetPh = PHControl::instance()->getCurrentPhTarget();
  float currentPh = (int)(PHProbe::instance()->getPh() * 100 + .5);
  currentPh = currentPh / 100;
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i.%i %i.%i %i.%i"), (int)currentPh, (int)(currentPh * 100) % 100,
             (int)currentTargetPh, (int)(currentTargetPh * 1000) % 1000, (int)overallTargetPh,
             (int)(overallTargetPh * 1000) % 1000);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}
