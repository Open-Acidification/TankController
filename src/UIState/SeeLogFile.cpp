/**
 * SeeLogFile.cpp
 */

#include "SeeLogFile.h"

#include "../TC_util.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/SD_TC.h"

void SeeLogFile::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char path[30];
  SD_TC::instance()->todaysDataFileName(path, sizeof(path));
  LiquidCrystal_TC::instance()->writeLine(path, 1);
}
