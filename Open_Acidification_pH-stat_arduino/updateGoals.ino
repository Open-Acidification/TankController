void updateGoals() {
  long now = millis();

  long phTime = now % phInterval;

  long phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, GOAL_RECORD_LENGTH);
  long phTimeNext = readLineFromSD("pt.txt", phSeriesPointer + 1 >= phSeriesSize ? 0 : phSeriesPointer + 1, GOAL_RECORD_LENGTH);

  // we want to make sure phTime is always between phTimeCurrent and phTimeNext
  if (phSeriesSize) {
    while ((phTimeNext > phTimeCurrent) && (phTimeNext < phTime)) {
      phSeriesPointer++;
      if (phSeriesPointer >= phSeriesSize) {
        phSeriesPointer = 0;
      }
      phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, GOAL_RECORD_LENGTH);
      phTimeNext = readLineFromSD("pt.txt", phSeriesPointer + 1 >= phSeriesSize ? 0 : phSeriesPointer + 1, GOAL_RECORD_LENGTH);
    }
    if ((phTimeNext < phTimeCurrent) && (phTimeCurrent > phTime)) {
      phSeriesPointer = 0;
      phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, GOAL_RECORD_LENGTH);
      phTimeNext = readLineFromSD("pt.txt", phSeriesPointer + 1 >= phSeriesSize ? 0 : phSeriesPointer + 1, GOAL_RECORD_LENGTH);
    }
  }
  EEPROM_writeDouble(PH_SERIES_POINTER_ADDRESS, phSeriesPointer);

  // interpolate ph goal
  long phValueCurrent = readLineFromSD("pv.txt", phSeriesPointer, GOAL_RECORD_LENGTH);
  long phValueNext;
  if (phSeriesPointer + 1 >= phSeriesSize) {
    phValueNext = readLineFromSD("pv.txt", 0, GOAL_RECORD_LENGTH);
    phTimeNext = phInterval;
  } else {
    phValueNext = readLineFromSD("pv.txt", phSeriesPointer + 1, GOAL_RECORD_LENGTH);
  }
  phset = phValueCurrent + (phValueNext - phValueCurrent) * (phTime - phTimeCurrent) / (phTimeNext - phTimeCurrent);

  // now do the same for temp
  long tempTime = now % tempInterval;

  long tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, GOAL_RECORD_LENGTH);
  long tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer + 1 >= tempSeriesSize ? 0 : tempSeriesPointer + 1, GOAL_RECORD_LENGTH);

  // we want to make sure tempTime is always between tempTimeCurrent and tempTimeNext
  if (tempSeriesSize) {
    while ((tempTimeNext > tempTimeCurrent) && (tempTimeNext < tempTime)) {
      tempSeriesPointer++;
      if (tempSeriesPointer >= tempSeriesSize) {
        tempSeriesPointer = 0;
      }
      tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, GOAL_RECORD_LENGTH);
      tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer + 1 >= tempSeriesSize ? 0 : tempSeriesPointer + 1, GOAL_RECORD_LENGTH);
    }
    if ((tempTimeNext < tempTimeCurrent) && (tempTimeCurrent > tempTime)) {
      tempSeriesPointer = 0;
      tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, GOAL_RECORD_LENGTH);
      tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer + 1 >= tempSeriesSize ? 0 : tempSeriesPointer + 1, GOAL_RECORD_LENGTH);
    }
  }
  EEPROM_writeDouble(TEMP_SERIES_POINTER_ADDRESS, tempSeriesPointer);

  // interpolate temp goal
  long tempValueCurrent = readLineFromSD("tv.txt", tempSeriesPointer, GOAL_RECORD_LENGTH);
  long tempValueNext;
  if (tempSeriesPointer + 1 >= tempSeriesSize) {
    tempValueNext = readLineFromSD("tv.txt", 0, GOAL_RECORD_LENGTH);
    tempTimeNext = tempInterval;
  } else {
    tempValueNext = readLineFromSD("tv.txt", tempSeriesPointer + 1, GOAL_RECORD_LENGTH);
  }
  tempset = tempValueCurrent + (tempValueNext - tempValueCurrent) * (tempTime - tempTimeCurrent) / (tempTimeNext - tempTimeCurrent);
}