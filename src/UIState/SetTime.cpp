/**
 * SetTime.cpp
 */

#include "SetTime.h"

#include "../Devices/DateTime_TC.h"
#include "../Devices/EEPROM_TC.h"
#include "../Devices/LiquidCrystal_TC.h"
#include "../Devices/Serial_TC.h"

void SetTime::setValue(double value) {
  values[subState++] = value;
  if (subState < NUM_VALUES) {
    clear();
    setNextState(this);
  } else {
    DateTime_TC dt(values[0], values[1], values[2], values[3], values[4]);
    dt.setAsCurrent();

    char buffer[20];
    strcpy(buffer, "YYYY-MM-DD hh:mm:ss");
    dt.toString(buffer);
    Serial_TC::instance()->print("Set time to ", buffer);

    DateTime_TC now = DateTime_TC::now();
    strcpy(buffer, "YYYY-MM-DD hh:mm");
    now.toString(buffer);
    LiquidCrystal_TC::instance()->writeLine("New Date/Time:  ", 0);
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    delay(1000);  // 1 second
    returnToMainMenu();
  }
}
