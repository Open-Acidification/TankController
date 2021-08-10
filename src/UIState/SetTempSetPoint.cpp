/**
 * SetTemperatureSetPoint.cpp
 */
#include "SetTempSetPoint.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TemperatureControl.h"

void SetTempSetPoint::setValue(float value) {
  TemperatureControl::instance()->setTargetTemperature(value);

  char output[17];
  snprintf(output, sizeof(output), "New Temp=%.2f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);  // after 3-second delay
}
