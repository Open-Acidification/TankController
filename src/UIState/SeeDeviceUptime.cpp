/**
 * SeeDeviceUptime.cpp
 */

#include "SeeDeviceUptime.h"

#include "Devices/DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "TC_util.h"

void SeeDeviceUptime::loop() {
  LiquidCrystal_TC::instance()->writeLine(DateTime_TC::now().as16CharacterString(), 0);
  uint32_t ms = millis();
  COUT("SeeDeviceUptime::loop() at " << ms);
  uint16_t days = ms / 86400000;
  uint16_t hours = (ms - (days * 86400000)) / 3600000;
  uint16_t minutes = (ms - (days * 86400000) - (hours * 3600000)) / 60000;
  uint16_t seconds = (ms - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000;
  char buffer[17];
  COUT("days: " << days << "; hours: " << hours << "; mins: " << minutes << "; secs: " << seconds);
  snprintf(buffer, sizeof(buffer), CSTR("Up d:%02i %02i:%02i:%02i"), days, hours, minutes, seconds);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  COUT(buffer);
}
