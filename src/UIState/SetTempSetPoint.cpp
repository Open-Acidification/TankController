/**
 * SetTemperatureSetPoint.cpp
 */
#include "SetTempSetPoint.h"

#include "LiquidCrystal_TC.h"
#include "TemperatureControl.h"

void SetTempSetPoint::setValue(double value) {
  TemperatureControl::instance()->setTargetTemperature(value);

  char output[17];
  snprintf(output, sizeof(output), "New Temp=%.2f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
