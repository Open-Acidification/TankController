/**
 * SetPHSetPoint.cpp
 */

#include "SetPHSetPoint.h"

#include "../Devices/EEPROM_TC.h"
#include "../Devices/LiquidCrystal_TC.h"

void SetPHSetPoint::setValue(double value) {
  EEPROM_TC::instance()->setPH(value);

  char output[17];
  sprintf(output, "New pH=%.4f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
