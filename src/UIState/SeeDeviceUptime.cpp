/**
 * SeeDeviceUptime.cpp
 */

#include "SeeDeviceUptime.h"

#include "DateTime_TC.h"
#include "LiquidCrystal_TC.h"
#include "TC_util.h"

void SeeDeviceUptime::loop() {
  unsigned long ms = millis();
  COUT("SeeDeviceUptime::loop() at " << ms);
  int days = ms / 86400000;
  int hours = (ms - (days * 86400000)) / 3600000;
  int minutes = (ms - (days * 86400000) - (hours * 3600000)) / 60000;
  int seconds = (ms - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000;
  char buffer[17];
  COUT("days: " << days << "; hours: " << hours << "; mins: " << minutes << "; secs: " << seconds);
  snprintf(buffer, sizeof(buffer), "Up d:%02i %02i:%02i:%02i", days, hours, minutes, seconds);
  LiquidCrystal_TC::instance()->writeLine(DateTime_TC::now().as16CharacterString(), 0);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  COUT(buffer);
}
