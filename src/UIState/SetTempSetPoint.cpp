/**
 * SetTemperatureSetPoint.cpp
 */
#include "SetTempSetPoint.h"

#include "../Devices/EEPROM_TC.h"
#include "../Devices/LiquidCrystal_TC.h"

void SetTempSetPoint::setValue(double value) {
  EEPROM_TC::instance()->setTemp(value);

  char output[17];
  sprintf(output, "New Temp=%.2f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  delay(1000);  // 1 second
  returnToMainMenu();
}
