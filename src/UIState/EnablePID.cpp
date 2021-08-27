/**
 * EnablePID.cpp
 */

#include "EnablePID.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"

float EnablePID::getCurrentValue() {
  return PHControl::instance()->getUsePID() ? 1 : 9;
}

void EnablePID::setValue(float value) {
  if (!(value == 1.0 || value == 9.0)) {
    LiquidCrystal_TC::instance()->writeLine("Invalid entry", 1);
  } else {
    bool flag = (uint16_t)value == 1;
    PHControl::instance()->enablePID(flag);
    if (flag) {
      LiquidCrystal_TC::instance()->writeLine("PID enabled", 1);
    } else {
      LiquidCrystal_TC::instance()->writeLine("PID disabled", 1);
    }
    returnToMainMenu(3000);
  }
}
