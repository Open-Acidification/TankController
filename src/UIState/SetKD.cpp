/**
 * SetKD.cpp
 */
#include "SetKD.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"

void SetKD::setValue(float value) {
  PID_TC::instance()->setKd(value);

  char output[17];
  snprintf(output, sizeof(output), "New KD=%.1f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);
}
