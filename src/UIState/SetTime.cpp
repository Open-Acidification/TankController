/**
 * SetTime.cpp
 */

#include "SetTime.h"

#include "wrappers/DateTime_TC.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/LiquidCrystal_TC.h"
#include "wrappers/Serial_TC.h"

SetTime::SetTime() : NumberCollectorState() {
  prompts[0] = F("Set Year (YYYY):");
  prompts[1] = F("Month (1-12):");
  prompts[2] = F("Day (1-31):");
  prompts[3] = F("Hour (0-23):    ");
  prompts[4] = F("Minute (0-59):  ");
}

float SetTime::getCurrentValue() {
  if (subState == 0) {
    // initialize current values with current date time
    DateTime_TC now = DateTime_TC::now();
    current[0] = now.year();
    current[1] = now.month();
    current[2] = now.day();
    current[3] = now.hour();
    current[4] = now.minute();
  }
  return current[subState];
}

void SetTime::setValue(float value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    start();
  } else {
    DateTime_TC dt(values[0], values[1], values[2], values[3], values[4]);
    dt.setAsCurrent();

    char* buffer = DateTime_TC::now().as16CharacterString();
    serial(F("Set time to %s"), buffer);
    LiquidCrystal_TC::instance()->writeLine(F("New Date/Time:"), 0);
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    returnToMainMenu(3000);  // after 3-second delay
  }
}
