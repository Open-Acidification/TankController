/**
 * SeeGoogleMins.cpp
 */

#include "SeeGoogleMins.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"

void SeeGoogleMins::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char buffer[17];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i"), EEPROM_TC::instance()->getGoogleSheetInterval());
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
