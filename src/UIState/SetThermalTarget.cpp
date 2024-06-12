/**
 * SetTemperatureSetPoint.cpp
 */
#include "SetThermalTarget.h"

#include "model/ThermalControl.h"
#include "wrappers/LiquidCrystal_TC.h"

SetThermalTarget::SetThermalTarget() : NumberCollector() {
  prompts[0] = F("Set Temperature");
  prompts[1] = F("Set ramp hours:");
}

float SetThermalTarget::getCurrentValue() {
  if (subState == 0) {
    return ThermalControl::instance()->getBaseThermalTarget();
  } else {
    uint32_t rampTimeStartSeconds = ThermalControl::instance()->getRampTimeStart();
    uint32_t rampTimeEndSeconds = ThermalControl::instance()->getRampTimeEnd();
    return (rampTimeEndSeconds - rampTimeStartSeconds) / 3600.0;
  }
}

void SetThermalTarget::setValue(float value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    start();
  } else {
    ThermalControl::instance()->setThermalTarget(values[0]);
    ThermalControl::instance()->setRampDurationHours(values[1]);
    char output0[17];
    char output1[17];
    snprintf_P(output0, sizeof(output0), (PGM_P)F("New Temp=%i.%02i"), (int)values[0],
               (int)(values[0] * 100 + 0.5) % 100);
    snprintf_P(output1, sizeof(output1), (PGM_P)F("New ramp=%i.%03i"), (int)values[1], (int)(values[1] * 1000) % 1000);
    LiquidCrystal_TC::instance()->writeLine(output0, 0);
    LiquidCrystal_TC::instance()->writeLine(output1, 1);
    returnToMainMenu(3000);  // after 3-second delay
  }
}
