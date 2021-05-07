/**
 * SetPHSetPoint.cpp
 */

#include "SetPHSetPoint.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/Serial_TC.h"

void SetPHSetPoint::setValue(double value) {
  PHControl::instance()->setTargetPh(value);

  char output[17];
  snprintf(output, sizeof(output), "New pH=%i.%04i", FLOAT(value, 4));
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(1000);  // after 1-second delay
}
