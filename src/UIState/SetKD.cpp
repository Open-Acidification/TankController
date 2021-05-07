/**
 * SetKD.cpp
 */
#include "SetKD.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"
#include "Devices/Serial_TC.h"

void SetKD::setValue(double value) {
  PID_TC::instance()->setKd(value);

  char output[17];
  snprintf(output, sizeof(output), "New KD=%5i.%1i", FLOAT(value, 1));
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}
