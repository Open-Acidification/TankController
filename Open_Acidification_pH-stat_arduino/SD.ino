void updateGoals() {
  long now = millis();

  long phTime = now % phInterval;

  long phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, goalRecordLength);
  long phTimeNext = readLineFromSD("pt.txt", phSeriesPointer + 1 >= phSeriesSize ? 0 : phSeriesPointer + 1, goalRecordLength);

  // we want to make sure phTime is always between phTimeCurrent and phTimeNext
  if (phSeriesSize) {
    while ((phTimeNext > phTimeCurrent) && (phTimeNext < phTime)) {
      phSeriesPointer++;
      if (phSeriesPointer >= phSeriesSize) {
        phSeriesPointer = 0;
      }
      phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, goalRecordLength);
      phTimeNext = readLineFromSD("pt.txt", phSeriesPointer + 1 >= phSeriesSize ? 0 : phSeriesPointer + 1, goalRecordLength);
    }
    if ((phTimeNext < phTimeCurrent) && (phTimeCurrent > phTime)) {
      phSeriesPointer = 0;
      phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, goalRecordLength);
      phTimeNext = readLineFromSD("pt.txt", phSeriesPointer + 1 >= phSeriesSize ? 0 : phSeriesPointer + 1, goalRecordLength);
    }
  }
  EEPROM_writeDouble(phSeriesPointerAddress, phSeriesPointer);

  // interpolate ph goal
  long phValueCurrent = readLineFromSD("pv.txt", phSeriesPointer, goalRecordLength);
  long phValueNext;
  if (phSeriesPointer + 1 >= phSeriesSize) {
    phValueNext = readLineFromSD("pv.txt", 0, goalRecordLength);
    phTimeNext = phInterval;
  } else {
    phValueNext = readLineFromSD("pv.txt", phSeriesPointer + 1, goalRecordLength);
  }
  phset = phValueCurrent + (phValueNext - phValueCurrent) * (phTime - phTimeCurrent) / (phTimeNext - phTimeCurrent);

  // now do the same for temp
  long tempTime = now % tempInterval;

  long tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, goalRecordLength);
  long tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer + 1 >= tempSeriesSize ? 0 : tempSeriesPointer + 1, goalRecordLength);

  // we want to make sure tempTime is always between tempTimeCurrent and tempTimeNext
  if (tempSeriesSize) {
    while ((tempTimeNext > tempTimeCurrent) && (tempTimeNext < tempTime)) {
      tempSeriesPointer++;
      if (tempSeriesPointer >= tempSeriesSize) {
        tempSeriesPointer = 0;
      }
      tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, goalRecordLength);
      tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer + 1 >= tempSeriesSize ? 0 : tempSeriesPointer + 1, goalRecordLength);
    }
    if ((tempTimeNext < tempTimeCurrent) && (tempTimeCurrent > tempTime)) {
      tempSeriesPointer = 0;
      tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, goalRecordLength);
      tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer + 1 >= tempSeriesSize ? 0 : tempSeriesPointer + 1, goalRecordLength);
    }
  }
  EEPROM_writeDouble(tempSeriesPointerAddress, tempSeriesPointer);

  // interpolate temp goal
  long tempValueCurrent = readLineFromSD("tv.txt", tempSeriesPointer, goalRecordLength);
  long tempValueNext;
  if (tempSeriesPointer + 1 >= tempSeriesSize) {
    tempValueNext = readLineFromSD("tv.txt", 0, goalRecordLength);
    tempTimeNext = tempInterval;
  } else {
    tempValueNext = readLineFromSD("tv.txt", tempSeriesPointer + 1, goalRecordLength);
  }
  tempset = tempValueCurrent + (tempValueNext - tempValueCurrent) * (tempTime - tempTimeCurrent) / (tempTimeNext - tempTimeCurrent);
}
void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
long readLineFromSD(char* fileName, int line, int lineLength) {
  long startingByte = line * (lineLength + 2);
  long lineValue;

  File tempFile = SD.open(fileName);
  if (tempFile.seek(startingByte)) {
    // fill up buffer
    char line[lineLength];
    memset(line, 0, lineLength);
    int i;
    for (i = 0; i < lineLength && tempFile.available(); i++) {
      byte read = tempFile.read();
      if ((char)read == '\n') {
        break;
      }
      line[i] = read;
    }
    line[i] = '\0';
    lineValue = atoi(line);
  } else {
    Serial.println("FAILED READ LINE SEEK");
  }

  // close the file:
  tempFile.close();

  return lineValue;
}
// ************************************************
// Log to SD card
// ************************************************

void LogToSD() {
  unsigned long SD_currentMillis = millis();
  DateTime now = rtc.now();
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.println("LOGGING TO SD");

  char* timeFormat = "YYYY/MM/DD/hh/YYMMDDhh.txt";
  char* linesFormat = "YYYY/MM/DD/hh/hh.txt";
  char* formattedFileName = now.toString(timeFormat);
  char* formattedLineFileName = now.toString(linesFormat);
  char* directoryFormat = "YYYY/MM/DD/hh";
  char* formattedDirectoryName = now.toString(directoryFormat);
  Serial.println(formattedDirectoryName);
  Serial.println(formattedFileName);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  SD.mkdir(formattedDirectoryName);

  // update accompanying line-count file
  File lineFile = SD.open(formattedLineFileName, FILE_WRITE);
  int lineFileSize = lineFile.size();
  if (!lineFileSize) {
    lineFile.println("1");
    lineFile.close();
  } else {
    char linesBuffer[10];
    lineFile.seek(0);
    byte read;
    for (int i = 0; i < 10 && read != -1; i++) {
      read = lineFile.read();
      linesBuffer[i] = (char)read;
    }
    int lineCount = atoi(linesBuffer);
    lineFile.close();
    SD.remove(formattedLineFileName);
    lineFile = SD.open(formattedLineFileName, FILE_WRITE);
    char lineCountBuffer[10];
    sprintf(lineCountBuffer, "%d", lineCount + 1);
    lineFile.println(lineCountBuffer);
    lineFile.close();
  }

  myFile = SD.open(formattedFileName, FILE_WRITE);
  // create information string with timestamp
  char formattedSDString[recordLength];
  memset(formattedSDString, 0, recordLength);
  char timeBuffer[100];
  memset(timeBuffer, 0, 100);
  char varBuffer[100];
  memset(varBuffer, 0, 100);
  DateTime tempNow = rtc.now();
  fmtDouble(tempNow.year(), 0, timeBuffer);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, "/");
  fmtDouble(tempNow.month(), 0, timeBuffer);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, "/");
  fmtDouble(tempNow.day(), 0, timeBuffer);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, " ");
  fmtDouble(tempNow.hour(), 0, timeBuffer);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, ":");
  if (tempNow.minute() < 10) {
    strcat(formattedSDString, "0");
  }
  fmtDouble(tempNow.minute(), 0, timeBuffer);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, ":");
  if (tempNow.second() < 10) {
    strcat(formattedSDString, "0");
  }
  fmtDouble(tempNow.second(), 0, timeBuffer);
  strcat(formattedSDString, timeBuffer);

  snprintf(varBuffer, 100, "%d", tankid);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  fmtDouble(temp, 2, varBuffer);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  fmtDouble(tempset, 2, varBuffer);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  fmtDouble(pH, 3, varBuffer);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  fmtDouble(phset, 3, varBuffer);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  fmtDouble(onTime, 0, varBuffer);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);

  Serial.print("formattedSDString SIZE: ");
  Serial.println(strlen(formattedSDString));
  Serial.print("ADDING: ");
  Serial.println(recordLength - strlen(formattedSDString));
  for (strlen(formattedSDString); strlen(formattedSDString) < recordLength;) {
    strcat(formattedSDString, " ");
  }
  Serial.print("AFTER EDIT formattedSDString SIZE: ");
  Serial.println(strlen(formattedSDString));
  myFile.println(formattedSDString);
  myFile.close();
  Serial.println(formattedSDString);
}
