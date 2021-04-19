/**
 * SeeGoogleMins.cpp
 */

#include "SeeGoogleMins.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"

void SeeGoogleMins::loop() {
  char buffer[17];
  sprintf(buffer, "%i", EEPROM_TC::instance()->getGoogleSheetInterval());
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
