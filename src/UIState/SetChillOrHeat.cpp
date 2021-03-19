/**
 * SetChillOrHeat.cpp
 */

#include "SetChillOrHeat.h"

#include "../Devices/EEPROM_TC.h"
#include "../Devices/LiquidCrystal_TC.h"

void SetChillOrHeat::setValue(double value) {
  if (!(value == 1.0 || value == 9.0)) {
    LiquidCrystal_TC::instance()->writeLine("Invalid entry!  ", 1);
  } else {
    bool isHeat = (int)value == 9;
    EEPROM_TC::instance()->setHeat(isHeat);
    if (isHeat) {
      LiquidCrystal_TC::instance()->writeLine("Use heater      ", 1);
    } else {
      LiquidCrystal_TC::instance()->writeLine("Use chiller     ", 1);
    }
  }
  delay(1000);  // 1 second
  returnToMainMenu();
}
