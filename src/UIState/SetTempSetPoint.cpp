/**
 * SetTemperatureSetPoint.cpp
 */
#include "SetTempSetPoint.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TemperatureControl.h"

SetTempSetPoint::SetTempSetPoint() : NumCollectorState() {
  prompts[0] = F("Set Temperature");
  prompts[1] = F("Set ramp hours:");
}

float SetTempSetPoint::getCurrentValue() {
  if (subState == 0) {
    return TemperatureControl::instance()->getBaseTargetTemperature();
  } else {
    uint32_t rampTimeStart = TemperatureControl::instance()->getRampTimeStart();
    uint32_t rampTimeEnd = TemperatureControl::instance()->getRampTimeEnd();
    return (rampTimeEnd - rampTimeStart) / 3600;
  }
}

void SetTempSetPoint::setValue(float value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    start();
  } else {
    TemperatureControl::instance()->setTargetTemperature(values[0]);
    TemperatureControl::instance()->setRampDuration(values[1]);
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
