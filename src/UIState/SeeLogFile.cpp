/**
 * SeeLogFile.cpp
 */

#include "SeeLogFile.h"

#include "model/TC_util.h"
#include "wrappers/LiquidCrystal_TC.h"
#include "wrappers/SD_TC.h"

void SeeLogFile::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char path[30];
  SD_TC::instance()->todaysDataFileName(path, sizeof(path));
  LiquidCrystal_TC::instance()->writeLine(path, 1);
}
