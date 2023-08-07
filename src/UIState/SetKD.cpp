/**
 * SetKD.cpp
 */
#include "SetKD.h"

#include "wrappers/LiquidCrystal_TC.h"
#include "wrappers/PID_TC.h"

void SetKD::setValue(float value) {
  PID_TC::instance()->setKd(value);

  char output[17];
  snprintf_P(output, sizeof(output), (PGM_P)F("New KD=%i.%i"), (int)value, (int)(value * 10 + 0.5) % 10);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);
}
