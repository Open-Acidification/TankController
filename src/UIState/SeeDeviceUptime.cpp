/**
 * SeeDeviceUptime.cpp
 */

#include "SeeDeviceUptime.h"

#include "model/TC_util.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/LiquidCrystal_TC.h"

void SeeDeviceUptime::loop() {
  LiquidCrystal_TC::instance()->writeLine(DateTime_TC::now().as16CharacterString(), 0);
  uint32_t ms = millis();
  uint16_t days = ms / 86400000;
  uint16_t hours = (ms - (days * 86400000)) / 3600000;
  uint16_t minutes = (ms - (days * 86400000) - (hours * 3600000)) / 60000;
  uint16_t seconds = (ms - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000;
  char buffer[17];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Up d:%02i %02i:%02i:%02i"), days, hours, minutes, seconds);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
