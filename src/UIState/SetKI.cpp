/**
 * SetKI.cpp
 */

#include "SetKI.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"

void SetKI::setValue(double value) {
  EEPROM_TC::instance()->setKI(value);

  char output[17];
  sprintf(output, "New KI=%.1f", value);  // "New KI=12345.5"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
