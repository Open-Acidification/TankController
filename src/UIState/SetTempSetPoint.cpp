/**
 * SetTemperatureSetPoint.cpp
 */
#include "SetTempSetPoint.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TemperatureControl.h"

void SetTempSetPoint::setValue(float value) {
  TemperatureControl::instance()->setTargetTemperature(value);

  char output[17];
  snprintf_P(output, sizeof(output), (PGM_P)F("New Temp=%i.%i"), (int)value, (int)(value * 100 + 0.5) % 100);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);  // after 3-second delay
}
