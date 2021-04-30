/**
 * SetTankID.cpp
 */

#include "SetTankID.h"

#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"

void SetTankID::setValue(double value) {
  EEPROM_TC::instance()->setTankID(value);

  char output[17];
  snprintf(output, sizeof(output), "Tank ID = %i", (int)value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
