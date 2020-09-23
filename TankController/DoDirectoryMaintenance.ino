void DoDirectoryMaintenance() {  // TODO: investigate bug with un-deletable files
  char name_buffer[20];
  char delete_buffer[20];
  memset(name_buffer, 0, 20);
  strcat(name_buffer, "/");
  DateTime now = rtc.now();
  int day = now.day();

  int year = now.year();
  Serial.println(F("CURRENT YEAR IS: "));
  Serial.println(year);
  char year_buffer[4];
  itoa(year, year_buffer, 10);
  strcat(name_buffer, year_buffer);
  Serial.println(F("NAME BUFFER IS: "));
  Serial.println(name_buffer);

  int month = now.month();
  Serial.println(F("CURRENT MONTH IS: "));
  Serial.println(month);
  strcat(name_buffer, "/");
  if (month < 10) {
    strcat(name_buffer, "0");
  }
  char month_buffer[12];
  itoa(month, month_buffer, 10);
  strcat(name_buffer, month_buffer);
  Serial.println(F("NAME BUFFER IS: "));
  Serial.println(name_buffer);
  File temp_dir = SD.open(name_buffer);

  // for relevant directories
  while (true) {
    File entry = temp_dir.openNextFile();
    if (entry) {
      Serial.println(F("ENTRY NAME IS: "));
      Serial.println(entry.name());
      // calculate elapsed time
      int dir_age_in_days = day - atoi(entry.name());
      Serial.println(F("ENTRY AGE IS: "));
      Serial.println(dir_age_in_days);
      // remove if directory is too old
      if (dir_age_in_days > max_data_age) {
        Serial.println(F("ENTRY IS TOO OLD"));
        memset(delete_buffer, 0, 20);
        strcat(delete_buffer, name_buffer);
        strcat(delete_buffer, "/");
        strcat(delete_buffer, entry.name());
        RecursiveRm(entry, delete_buffer);
      }
    } else {
      break;
    }
  }
  Serial.println(F("max_data_age IS: "));
  Serial.println(max_data_age);
}
