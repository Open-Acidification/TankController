/**
 * SetTime.cpp
 */

#include "SetTime.h"

#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"
#include "Serial_TC.h"

void SetTime::setValue(double value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    start();
  } else {
    DateTime_TC dt(values[0], values[1], values[2], values[3], values[4]);
    dt.setAsCurrent();

    char *buffer = DateTime_TC::now().as16CharacterString();
    Serial_TC::instance()->printf("Set time to %s", buffer);
    LiquidCrystal_TC::instance()->writeLine("New Date/Time:  ", 0);
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    returnToMainMenu(1000);  // after 1-second delay
  }
}
