/**
 * SeePHSlope.cpp
 */

#include "SeePHSlope.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"

int SeePHSlope::writeToBuffer(char *buffer, int size, int line) {
  int offs = 0;
  if (size > 20) {
    offs += snprintf_P(buffer, size, (PGM_P)F("\"PHSlope\":\""));
  }
  PHProbe::instance()->getSlope(buffer + offs, 20);  // NOT null-terminated if non-empty!
  if (size > 20) {
    // If empty don't increment offset
    offs += (buffer[offs] == '\0' ? 0 : 20);
    buffer[offs++] = '\"';
    buffer[offs] = '\0';
  }
  return offs;
}
void SeePHSlope::loop() {
  char buffer[20];
  writeToBuffer(buffer, 20, 1);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}

void SeePHSlope::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine(F("requesting slope"), 1);
  PHProbe::instance()->sendSlopeRequest();
}
