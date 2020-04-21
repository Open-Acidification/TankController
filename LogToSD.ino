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
  SDstring = "";
  SDstring += String((char*) now.month());
  SDstring += "/";
  SDstring += String((char*) now.day());
  SDstring += "/";
  SDstring += String((char*) now.year());
  SDstring += " ";
  SDstring += String((char*) now.hour());
  SDstring += ":";
  if (now.minute() < 10) {
    SDstring += "0";
  }
  SDstring += String((char*) now.minute());
  SDstring += ":";
  if (now.second() < 10) {
    SDstring += "0";
  }
  SDstring += String((char*) now.second());
  SDstring += ",";
  SDstring += tankid;
  SDstring += ",";
  SDstring += String(temp, 2);
  SDstring += ",";
  SDstring += String(tempset, 2);
  SDstring += ",";
  SDstring += String(pH, 3);
  SDstring += ",";
  SDstring += String(phset, 3);
  SDstring += ",";
  SDstring += String(onTime);
  SDstring += ",";
  SDstring += Kp;
  SDstring += ",";
  SDstring += Ki;
  SDstring += ",";
  SDstring += Kd;
  myFile.println(SDstring);
  myFile.close();
  Serial.println(SDstring);
}