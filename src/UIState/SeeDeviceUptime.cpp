/**
 * SeeDeviceUptime.cpp
 */

#include "SeeDeviceUptime.h"

#include "Devices/DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"

void SeeDeviceUptime::start() {
  int days = floor(millis() / 86400000);
  int hours = floor((millis() - (days * 86400000)) / 3600000);
  int minutes = floor((millis() - (days * 86400000) - (hours * 3600000)) / 60000);
  int seconds = floor((millis() - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000);
  char buffer[17];
  sprintf(buffer, "Up d:%02i %02i:%02i:%02i", days, hours, minutes, seconds);
  LiquidCrystal_TC::instance()->writeLine(DateTime_TC::now().as16CharacterString(), 0);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
