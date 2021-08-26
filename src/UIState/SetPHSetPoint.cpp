/**
 * SetPHSetPoint.cpp
 */

#include "SetPHSetPoint.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "TC_util.h"

float SetPHSetPoint::getCurrentValue() {
  return PHControl::instance()->getTargetPh();
}

void SetPHSetPoint::setValue(float value) {
  PHControl::instance()->setTargetPh(value);

  char output[17];
  snprintf(output, sizeof(output), CSTR("New pH=%.3f"), value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);  // after 3-second delay
}
