// ************************************************
// Log to SD card
// ************************************************

void LogToSD() {
  unsigned long SD_currentMillis = millis();
  DateTime now = rtc.now();
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  if (SD_currentMillis - SD_previousMillis >= SD_interval) {
    SD_previousMillis = SD_currentMillis;
    filename = String(now.year() - 2000) + "-" + now.month() + "-" + now.day();
    file_full = filename + ".txt";

    myFile = SD.open(file_full, FILE_WRITE);
    myFile.println("time,temp,temp setpoint,pH,pH setpoint");
    myFile.close();
  }

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  myFile = SD.open(file_full, FILE_WRITE);
  SDstring = "";
  SDstring += String(now.month(), DEC);
  SDstring += "/";
  SDstring += String(now.day(), DEC);
  SDstring += "/";
  SDstring += String(now.year(), DEC);
  SDstring += " ";
  SDstring += String(now.hour(), DEC);
  SDstring += ":";
  if (now.minute() < 10) {
    SDstring += "0";
  }
  SDstring += String(now.minute(), DEC);
  SDstring += ":";
  if (now.second() < 10) {
    SDstring += "0";
  }
  SDstring += String(now.second(), DEC);
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