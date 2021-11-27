/**
 * SetPHWithSine.cpp
 */

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "SetPHWithSine.h"


SetPHWithSine::SetPHWithSine(TankController* tc) : NumCollectorState(tc) {
  prompts[0] = F("Set pH Set Point");
  prompts[1] = F("Set Amplitude:");
  prompts[2] = F("Set Period hrs:");
}

float SetPHWithSine::getCurrentValue() {
  if (subState == 0) {
    return PHControl::instance()->getTargetPh();
  } else if (subState == 1) {
    if (PHControl::instance()->getPHSetType() == PHControl::instance()->phSetTypeTypes::SINE_TYPE) {
      return PHControl::instance()->getAmplitude();
    } else {
      return 0.000;
    }
  } else {
    if (PHControl::instance()->getPHSetType() == PHControl::instance()->phSetTypeTypes::SINE_TYPE) {
      return PHControl::instance()->getPeriod() / 3600;
    } else {
      return 0.0;
    }
  }
}

void SetPHWithSine::setValue(float value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    start();
  } else {
    PHControl::instance()->setTargetPh(values[0]);
    PHControl::instance()->setSine(values[1], values[2]);
    char output0[17];
    char temp1[8];
    char temp2[9];
    char output1[17];
    snprintf_P(output0, sizeof(output0), (PGM_P)F("New pH=%i.%i"), (int)values[0],
               (int)(values[0] * 1000 + 0.5) % 1000);
    snprintf_P(temp1, sizeof(temp1), (PGM_P)F("A=%i.%i"), (int)values[1], (int)(values[1] * 1000) % 1000);
    snprintf_P(temp2, sizeof(temp2), (PGM_P)F(" P=%i.%i"), (int)values[2], (int)(values[2] * 1000) % 1000);
    snprintf_P(output1, sizeof(output1), (PGM_P)F("%s%s"), (const char*)temp1, (const char*)temp2);
    LiquidCrystal_TC::instance()->writeLine(output0, 0);
    LiquidCrystal_TC::instance()->writeLine(output1, 1);
    returnToMainMenu(3000);  // after 3-second delay
  }
}
