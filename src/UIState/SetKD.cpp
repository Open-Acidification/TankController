/**
 * SetKD.cpp
 */

#include "SetKD.h"

#include "../Devices/EEPROM_TC.h"
#include "../Devices/LiquidCrystal_TC.h"

void SetKD::setValue(double value) {
  EEPROM_TC::instance()->setKD(value);

  char output[17];
  sprintf(output, "New KD=%.1f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
