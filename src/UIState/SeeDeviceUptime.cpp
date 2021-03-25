/**
 * SeeDeviceUptime.cpp
 */

#include "SeeDeviceUptime.h"

#include "Devices/DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "TC_util.h"

void SeeDeviceUptime::loop() {
  COUT("loop()");
  unsigned long ms = millis();
  int days = floor(ms / 86400000);
  int hours = floor((ms - (days * 86400000)) / 3600000);
  int minutes = floor((ms - (days * 86400000) - (hours * 3600000)) / 60000);
  int seconds = floor((ms - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000);
  char buffer[17];
  sprintf(buffer, "Up d:%02i %02i:%02i:%02i", days, hours, minutes, seconds);
  LiquidCrystal_TC::instance()->writeLine(DateTime_TC::now().as16CharacterString(), 0);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  COUT(buffer);
  if (msEnd < millis()) {
    COUT("done");
    returnToMainMenu();
  }
}
