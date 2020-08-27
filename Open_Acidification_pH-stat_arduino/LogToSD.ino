// ************************************************
// Log to SD card
// ************************************************

void LogToSD() {
  unsigned long SD_currentMillis = millis();
  DateTime now = rtc.now();
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.println(F("LOGGING TO SD"));

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
  char formattedSDString[RECORD_LENGTH];
  memset(formattedSDString, 0, RECORD_LENGTH);
  char timeBuffer[100];
  memset(timeBuffer, 0, 100);
  char varBuffer[100];
  memset(varBuffer, 0, 100);
  DateTime tempNow = rtc.now();
  FormatDouble(tempNow.year(), 0, timeBuffer, 0xffff);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, "/");
  FormatDouble(tempNow.month(), 0, timeBuffer, 0xffff);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, "/");
  FormatDouble(tempNow.day(), 0, timeBuffer, 0xffff);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, " ");
  FormatDouble(tempNow.hour(), 0, timeBuffer, 0xffff);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, ":");
  if (tempNow.minute() < 10) {
    strcat(formattedSDString, "0");
  }
  FormatDouble(tempNow.minute(), 0, timeBuffer, 0xffff);
  strcat(formattedSDString, timeBuffer);
  strcat(formattedSDString, ":");
  if (tempNow.second() < 10) {
    strcat(formattedSDString, "0");
  }
  FormatDouble(tempNow.second(), 0, timeBuffer, 0xffff);
  strcat(formattedSDString, timeBuffer);

  snprintf(varBuffer, 100, "%d", tankid);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  FormatDouble(temp, 2, varBuffer, 0xffff);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  FormatDouble(tempset, 2, varBuffer, 0xffff);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  FormatDouble(pH, 3, varBuffer, 0xffff);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  FormatDouble(phset, 3, varBuffer, 0xffff);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);
  FormatDouble(onTime, 0, varBuffer, 0xffff);
  strcat(formattedSDString, ",");
  strcat(formattedSDString, varBuffer);

  Serial.print(F("formattedSDString SIZE: "));
  Serial.println(strlen(formattedSDString));
  Serial.print(F("ADDING: "));
  Serial.println(RECORD_LENGTH - strlen(formattedSDString));
  for (strlen(formattedSDString); strlen(formattedSDString) < RECORD_LENGTH;) {
    strcat(formattedSDString, " ");
  }
  Serial.print(F("AFTER EDIT formattedSDString SIZE: "));
  Serial.println(strlen(formattedSDString));
  myFile.println(formattedSDString);
  myFile.close();
  Serial.println(formattedSDString);
}