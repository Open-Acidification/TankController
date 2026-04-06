/**
 * ResetEEPROM.cpp
 */
#include "ResetEEPROM.h"

#include "Wait.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/LiquidCrystal_TC.h"

void ResetEEPROM::handleKey(char key) {
  switch (key) {
    case 'A':  // Save (erase EEPROM)
      EEPROM_TC::instance()->resetAll();
      returnToMainMenu();
      break;
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
