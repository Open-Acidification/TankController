/**
 * SetTemperatureSetPoint.cpp
 */
#include "SetTempSetPoint.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/Serial_TC.h"
#include "Devices/TemperatureControl.h"

void SetTempSetPoint::setValue(double value) {
  TemperatureControl::instance()->setTargetTemperature(value);

  char output[17];
  snprintf(output, sizeof(output), "New Temp=%02i.%02i", FLOAT(value, 2));
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
