/**
 * SetKP.cpp
 */
#include "SetKP.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"
#include "TC_util.h"

void SetKP::setValue(float value) {
  PID_TC::instance()->setKp(value);
  char output[17];
  snprintf(output, sizeof(output), CSTR("New KP=%.1f"), value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);
}
