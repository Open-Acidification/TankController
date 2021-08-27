/**
 * SetPHSetPoint.cpp
 */

#include "SetPHSetPoint.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"

float SetPHSetPoint::getCurrentValue() {
  return PHControl::instance()->getTargetPh();
}

void SetPHSetPoint::setValue(float value) {
  PHControl::instance()->setTargetPh(value);

  char output[17];
  snprintf_P(output, sizeof(output), (PGM_P)F("New pH=%.3f"), value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);  // after 3-second delay
}
