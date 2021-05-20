/**
 * SetChillOrHeat.cpp
 */

#include "SetChillOrHeat.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TemperatureControl.h"

void SetChillOrHeat::setValue(float value) {
  if (!(value == 1.0 || value == 9.0)) {
    LiquidCrystal_TC::instance()->writeLine("Invalid entry!  ", 1);
  } else {
    bool isHeat = (uint16_t)value == 9;
    TemperatureControl::instance()->enableHeater(isHeat);
    if (isHeat) {
      LiquidCrystal_TC::instance()->writeLine("Use heater      ", 1);
    } else {
      LiquidCrystal_TC::instance()->writeLine("Use chiller     ", 1);
    }
  }
  returnToMainMenu(1000);  // after 1-second delay
}
