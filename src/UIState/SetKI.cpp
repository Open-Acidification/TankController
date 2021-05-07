/**
 * SetKI.cpp
 */
#include "SetKI.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"
#include "Devices/Serial_TC.h"

void SetKI::setValue(double value) {
  PID_TC::instance()->setKi(value);

  char output[17];
  snprintf(output, sizeof(output), "New KI=%5i.%1i", FLOAT(value, 1));  // "New KI=12345.5"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}
