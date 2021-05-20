/**
 * SetGoogleSheetInterval.cpp
 */

#include "SetGoogleSheetInterval.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"

void SetGoogleSheetInterval::setValue(float value) {
  EEPROM_TC::instance()->setGoogleSheetInterval((uint16_t)value);

  char output[17];
  snprintf(output, sizeof(output), "New interval=%i", (uint16_t)value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
