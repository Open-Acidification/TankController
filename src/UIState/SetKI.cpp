/**
 * SetKI.cpp
 */
#include "SetKI.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"

void SetKI::setValue(float value) {
  PID_TC::instance()->setKi(value);

  char output[17];
  snprintf_P(output, sizeof(output), (PGM_P)F("New KI=%.1f"), value);  // "New KI=12345.5"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);
}
