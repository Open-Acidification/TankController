/**
 * SetKI.cpp
 */
#include "SetKI.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"

void SetKI::setValue(float value) {
  PID_TC::instance()->setKi(value);

  char output[17];
  snprintf_P(output, sizeof(output), (PGM_P)F("New KI=%i.%i"), (int)value,
             (int)(value * 10 + 0.5) % 10);  // "New KI=12345.5"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);
}
