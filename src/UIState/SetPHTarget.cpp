/**
 * SetPHTarget.cpp
 */

#include "SetPHTarget.h"

#include "model/PHControl.h"
#include "wrappers/LiquidCrystal_TC.h"

SetPHTarget::SetPHTarget() : NumberCollector() {
  prompts[0] = F("Set pH Set Point");
  prompts[1] = F("Set ramp hours:");
}

float SetPHTarget::getCurrentValue() {
  if (subState == 0) {
    return PHControl::instance()->getBaseTargetPh();
  } else {
    uint32_t rampTimeStartSeconds = PHControl::instance()->getPhRampTimeStart();
    uint32_t rampTimeEndSeconds = PHControl::instance()->getPhRampTimeEnd();
    return (rampTimeEndSeconds - rampTimeStartSeconds) / 3600.0;
  }
}

void SetPHTarget::setValue(float value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    start();
  } else {
    PHControl::instance()->setBaseTargetPh(values[0]);
    PHControl::instance()->setRampDurationHours(values[1]);
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
