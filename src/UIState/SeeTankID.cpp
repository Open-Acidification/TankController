/**
 * SeeTankID.cpp
 */

#include "SeeTankID.h"

#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"

void SeeTankID::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char id[17];
  snprintf(id, sizeof(id), "%i", EEPROM_TC::instance()->getTankID());
  LiquidCrystal_TC::instance()->writeLine(id, 1);
}
