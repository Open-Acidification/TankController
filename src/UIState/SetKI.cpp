/**
 * SetKI.cpp
 */

#include "SetKI.h"

#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"

void SetKI::setValue(double value) {
  EEPROM_TC::instance()->setKI(value);

  char output[17];
  snprintf(output, sizeof(output), "New KI=%.1f", value);  // "New KI=12345.5"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
