/**
 * SeeLogFile.cpp
 */

#include "SeeLogFile.h"

#include "../TC_util.h"
#include "LiquidCrystal_TC.h"
#include "SD_TC.h"

void SeeLogFile::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char fileName[10];
  SD_TC::instance()->todaysDataFileName().substring(11).toCharArray(fileName, 9);
  fileName[9] = '\0';
  LiquidCrystal_TC::instance()->writeLine(fileName, 1);
}
