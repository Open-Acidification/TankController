/**
 * SetKD.cpp
 */

#include "SetKD.h"

#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"

void SetKD::setValue(double value) {
  EEPROM_TC::instance()->setKD(value);

  char output[17];
  snprintf(output, sizeof(output), "New KD=%.1f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
