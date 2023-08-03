/**
 * SetPHSetPoint.cpp
 */

#include "SetPHSetPoint.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"

SetPHSetPoint::SetPHSetPoint() : NumberCollectorState() {
  prompts[0] = F("Set pH Set Point");
  prompts[1] = F("Set ramp hours:");
}

float SetPHSetPoint::getCurrentValue() {
  if (subState == 0) {
    return PHControl::instance()->getBaseTargetPh();
  } else {
    uint32_t rampTimeStart = PHControl::instance()->getPhRampTimeStart();
    uint32_t rampTimeEnd = PHControl::instance()->getPhRampTimeEnd();
    return (rampTimeEnd - rampTimeStart) / 3600;
  }
}

void SetPHSetPoint::setValue(float value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    start();
  } else {
    PHControl::instance()->setBaseTargetPh(values[0]);
    PHControl::instance()->setRampDuration(values[1]);
    char output0[17];
    char output1[17];
    snprintf_P(output0, sizeof(output0), (PGM_P)F("New pH=%i.%03i"), (int)values[0],
               (int)(values[0] * 1000 + 0.5) % 1000);
    snprintf_P(output1, sizeof(output1), (PGM_P)F("New ramp=%i.%03i"), (int)values[1], (int)(values[1] * 1000) % 1000);
    LiquidCrystal_TC::instance()->writeLine(output0, 0);
    LiquidCrystal_TC::instance()->writeLine(output1, 1);
    returnToMainMenu(3000);  // after 3-second delay
  }
}
