/**
 * SeeTankID.cpp
 */

#include "SeeTankID.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"

int SeeTankID::writeToBuffer(char *buffer, int size, int line) {
  if (size > 17) {
    return snprintf_P(buffer, size, (PGM_P)F("\"TankID\":%i"), EEPROM_TC::instance()->getTankID());
  } else {
    return snprintf_P(buffer, size, (PGM_P)F("%i"), EEPROM_TC::instance()->getTankID());
  }
}

void SeeTankID::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char id[17];
  writeToBuffer(id, sizeof(id), 1);
  LiquidCrystal_TC::instance()->writeLine(id, 1);
}
