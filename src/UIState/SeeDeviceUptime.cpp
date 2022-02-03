/**
 * SeeDeviceUptime.cpp
 */

#include "SeeDeviceUptime.h"

#include "Devices/DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "TC_util.h"

int SeeDeviceUptime::writeToBuffer(char* buffer, int size, int line) {
  uint32_t ms = millis();
  uint16_t days = ms / 86400000;
  uint16_t hours = (ms - (days * 86400000)) / 3600000;
  uint16_t minutes = (ms - (days * 86400000) - (hours * 3600000)) / 60000;
  uint16_t seconds = (ms - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000;
  COUT("days: " << days << "; hours: " << hours << "; mins: " << minutes << "; secs: " << seconds);
  if (size > 17) {
    return snprintf_P(buffer, size, (PGM_P)F("\"Uptime\":\"%id %ih %im %is\""), days, hours, minutes, seconds);
  } else {
    return snprintf_P(buffer, size, (PGM_P)F("Up d:%02i %02i:%02i:%02i"), days, hours, minutes, seconds);
  }
}
void SeeDeviceUptime::loop() {
  LiquidCrystal_TC::instance()->writeLine(DateTime_TC::now().as16CharacterString(), 0);
  char buffer[17];
  writeToBuffer(buffer, sizeof(buffer), 1);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  COUT(buffer);
}
