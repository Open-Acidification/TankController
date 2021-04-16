/**
 * SetKP.cpp
 */

#include "SetKP.h"

#include "../Devices/EEPROM_TC.h"
#include "../Devices/LiquidCrystal_TC.h"

void SetKP::setValue(double value) {
  EEPROM_TC::instance()->setKP(value);

  char output[17];
  sprintf(output, "New KP=%.1f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
