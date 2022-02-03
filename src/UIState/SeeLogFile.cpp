/**
 * SeeLogFile.cpp
 */

#include "SeeLogFile.h"

#include "../TC_util.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/SD_TC.h"
int SeeLogFile::writeToBuffer(char *buffer, int size, int line) {
  int offs = 0;
  if (size > 30) {
    offs += snprintf_P(buffer, size, (PGM_P)F("\"LogFile\":\""));
  }
  SD_TC::instance()->todaysDataFileName(buffer + offs, size);
  if (offs) {
    // Final padding of quotes for JSON
    buffer[offs++] = '\"';
    buffer[offs] = '\0';
  }
  return offs;
}

void SeeLogFile::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char path[30];
  writeToBuffer(path, sizeof(path), 1);
  LiquidCrystal_TC::instance()->writeLine(path, 1);
}
