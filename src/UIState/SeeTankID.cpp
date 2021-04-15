/**
 * SeeTankID.cpp
 */

#include "SeeTankID.h"

#include "../Devices/EEPROM_TC.h"
#include "../Devices/LiquidCrystal_TC.h"

void SeeTankID::loop() {
  char id[17];
  sprintf(id, "%i", EEPROM_TC::instance()->getTankID());
  LiquidCrystal_TC::instance()->writeLine(id, 1);
}
