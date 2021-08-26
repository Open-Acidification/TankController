/**
 * SetKI.cpp
 */
#include "SetKI.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"
#include "TC_util.h"

void SetKI::setValue(float value) {
  PID_TC::instance()->setKi(value);

  char output[17];
  snprintf(output, sizeof(output), CSTR("New KI=%.1f"), value);  // "New KI=12345.5"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);
}
