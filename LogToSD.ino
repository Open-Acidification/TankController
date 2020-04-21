// ************************************************
// Log to SD card
// ************************************************

void LogToSD() {
  unsigned long SD_currentMillis = millis();
  DateTime now = rtc.now();
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.println("LOGGING TO SD");

  char formattedFileName[12];
  char* timeFormat = "YYYY/MM/DD/hh/YYMMDDhh.txt";
  char* formattedTime = now.toString(timeFormat);
  char* directoryFormat = "YYYY/MM/DD/hh";
  char* formattedDirectoryName = now.toString(directoryFormat);
  strcpy(formattedFileName, formattedTime);
  Serial.println(formattedDirectoryName);
  Serial.println(formattedFileName);
  
  if (SD_currentMillis - SD_previousMillis >= SD_interval) {
    SD_previousMillis = SD_currentMillis;
    SD.mkdir(formattedDirectoryName);

    myFile = SD.open(formattedFileName, FILE_WRITE);
    myFile.println("time,temp,temp setpoint,pH,pH setpoint");
    myFile.close();
  }

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  SD.mkdir(formattedDirectoryName);
  myFile = SD.open(formattedFileName, FILE_WRITE);
  char SDstring[200];
  strcat(SDstring, (char*) now.month());
  strcat(SDstring, "/");
  strcat(SDstring, (char*) now.day());
  strcat(SDstring, "/");
  strcat(SDstring, (char*) now.year());
  strcat(SDstring, " ");
  strcat(SDstring, (char*) now.hour());
  strcat(SDstring, ":");
  if (now.minute() < 10) {
    strcat(SDstring, "0");
  }
  strcat(SDstring, (char*) now.minute());
  strcat(SDstring, ":");
  if (now.second() < 10) {
    strcat(SDstring, "0");
  }
  strcat(SDstring, (char*) now.second());
  strcat(SDstring, ",");
  strcat(SDstring, (char*) tankid);
  strcat(SDstring, ",");
  strcat(SDstring, (char*) temp);
  strcat(SDstring, ",");
  strcat(SDstring, (char*) tempset);
  strcat(SDstring, ",");
  strcat(SDstring, (char*) pH);
  strcat(SDstring, ",");
  strcat(SDstring, (char*) phset);
  strcat(SDstring, ",");
  strcat(SDstring, (char*) onTime);
  strcat(SDstring, ",");
  strcat(SDstring, (char*) Kp);
  strcat(SDstring, ",");
  strcat(SDstring, (char*) Ki);
  strcat(SDstring, ",");
  strcat(SDstring, (char*) Kd);
  myFile.println(SDstring);
  myFile.close();
  Serial.println(SDstring);
}