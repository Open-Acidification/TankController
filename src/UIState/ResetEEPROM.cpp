/**
 * ResetEEPROM.cpp
 */
#include "ResetEEPROM.h"

#include <avr/wdt.h>

#include "Wait.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/LiquidCrystal_TC.h"

void ResetEEPROM::handleKey(char key) {
  switch (key) {
    case 'A':  // Save (erase EEPROM)
      EEPROM_TC::instance()->resetEEPROM();
      wdt_enable(WDTO_15MS);
      do {
      } while (true);
      break;
    case 'D':  // Don't save (cancel)
      wdt_enable(WDTO_15MS);
      do {
      } while (true);
      break;
    default:
      break;
  };
}

void ResetEEPROM::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine(F("D: Cancel"), 1);
}
