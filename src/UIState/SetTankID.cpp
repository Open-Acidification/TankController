/**
 * SetTankID.cpp
 */

#include "SetTankID.h"

#include "wrappers/EEPROM_TC.h"
#include "wrappers/LiquidCrystal_TC.h"

void SetTankID::setValue(float value) {
  EEPROM_TC::instance()->setTankID(value);

  char output[17];
  snprintf_P(output, sizeof(output), (PGM_P)F("Tank ID = %i"), (uint16_t)value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);  // after 3-second delay
}
