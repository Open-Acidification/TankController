/**
 * SeeGoogleMins.cpp
 */

#include "SeeGoogleMins.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"
int SeeGoogleMins::writeToBuffer(char* buffer, int size, int line) {
  if (size > 17) {
    return snprintf_P(buffer, size, (PGM_P)F("\"GoogleSheetInterval\":%i"),
                      EEPROM_TC::instance()->getGoogleSheetInterval());
  } else {
    return snprintf_P(buffer, size, (PGM_P)F("%i"), EEPROM_TC::instance()->getGoogleSheetInterval());
  }
}

void SeeGoogleMins::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char buffer[17];
  writeToBuffer(buffer, sizeof(buffer), 1);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
