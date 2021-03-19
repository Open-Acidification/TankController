/**
 * SetTankID.cpp
 */

#include "SetTankID.h"

#include "../Devices/EEPROM_TC.h"
#include "../Devices/LiquidCrystal_TC.h"

void SetTankID::setValue(double value) {
  EEPROM_TC::instance()->setTankID(value);

  char output[17];
  sprintf(output, "Tank ID = %i", (int)value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
