/**
 * SetTime.cpp
 */

#include "SetTime.h"

#include "Devices/DateTime_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"

SetTime::SetTime(TankControllerLib *tc) : NumCollectorState(tc) {
  prompts[0] = CSTR("Set Year (YYYY):");
  prompts[1] = CSTR("Month (1-12):");
  prompts[2] = CSTR("Day (1-31):");
  prompts[3] = CSTR("Hour (0-23):");
  prompts[4] = CSTR("Minute (0-59):");
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
    LiquidCrystal_TC::instance()->writeLine(CSTR("New Date/Time:"), 0);

    DateTime_TC dt(values[0], values[1], values[2], values[3], values[4]);
    dt.setAsCurrent();
    char *buffer = DateTime_TC::now().as16CharacterString();
    serial(CSTR("Set time to %s"), buffer);
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    returnToMainMenu(3000);  // after 3-second delay
  }
}
