/**
 * SetChillOrHeat.cpp
 */

#include "SetChillOrHeat.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TemperatureControl.h"

float SetChillOrHeat::getCurrentValue() {
  return EEPROM_TC::instance()->getHeat() ? 9 : 1;
}

void SetChillOrHeat::setValue(float value) {
  if (!(value == 1.0 || value == 9.0)) {
    LiquidCrystal_TC::instance()->writeLine(F("Invalid entry!"), 1);
  } else {
    bool isHeat = (uint16_t)value == 9;
    TemperatureControl::instance()->enableHeater(isHeat);
    if (isHeat) {
      LiquidCrystal_TC::instance()->writeLine(F("Use heater"), 1);
    } else {
      LiquidCrystal_TC::instance()->writeLine(F("Use chiller"), 1);
    }
  }
  returnToMainMenu(3000);  // after 1-second delay
}
