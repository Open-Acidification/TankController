/**
 * SetKP.cpp
 */
#include "Devices/PID_TC.h"

#include "SetKP.h"
#include "Devices/LiquidCrystal_TC.h"

void SetKP::setValue(double value) {
  PID_TC::instance()->setKp(value);
  char output[17];
  snprintf(output, sizeof(output), "New KP=%.1f", value);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}
