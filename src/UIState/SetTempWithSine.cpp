/**
 * SetTempWithSine.cpp
 */

#include "SetTempWithSine.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/ThermalControl.h"

SetTempWithSine::SetTempWithSine() : NumberCollectorState() {
  prompts[0] = F("Set T Set Point");
  prompts[1] = F("Set Amplitude:");
  prompts[2] = F("Set Period hrs:");
}

float SetTempWithSine::getCurrentValue() {
  if (subState == 0) {
    return ThermalControl::instance()->getBaseThermalTarget();
  } else if (subState == 1) {
    if (ThermalControl::instance()->getThermalFunctionType() ==
        ThermalControl::instance()->thermalFunctionTypes::SINE_TYPE) {
      return ThermalControl::instance()->getAmplitude();
    } else {
      return 0.000;
    }
  } else {
    if (ThermalControl::instance()->getThermalFunctionType() ==
        ThermalControl::instance()->thermalFunctionTypes::SINE_TYPE) {
      return ThermalControl::instance()->getPeriod() / 3600;
    } else {
      return 0.0;
    }
  }
}

void SetTempWithSine::setValue(float value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    start();
  } else {
    ThermalControl::instance()->setTargetTemperature(values[0]);
    ThermalControl::instance()->setSine(values[1], values[2]);
    char output0[17];
    char temp1[8];
    char temp2[9];
    char output1[17];
    snprintf_P(output0, sizeof(output0), (PGM_P)F("New Temp=%i.%02i"), (int)values[0],
               (int)(values[0] * 100 + 0.5) % 100);
    snprintf_P(temp1, sizeof(temp1), (PGM_P)F("A=%i.%02i"), (int)values[1], (int)(values[1] * 100 + 0.5) % 100);
    snprintf_P(temp2, sizeof(temp2), (PGM_P)F(" P=%i.%03i"), (int)values[2], (int)(values[2] * 1000 + 0.5) % 1000);
    snprintf_P(output1, sizeof(output1), (PGM_P)F("%s%s"), (const char*)temp1, (const char*)temp2);
    LiquidCrystal_TC::instance()->writeLine(output0, 0);
    LiquidCrystal_TC::instance()->writeLine(output1, 1);
    returnToMainMenu(3000);  // after 3-second delay
  }
}
