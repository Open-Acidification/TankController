/**
 * SeeGoogleMins.cpp
 */

#include "SeeGoogleMins.h"

#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"

void SeeGoogleMins::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char buffer[17];
  snprintf(buffer, sizeof(buffer), "%i", EEPROM_TC::instance()->getGoogleSheetInterval());
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
