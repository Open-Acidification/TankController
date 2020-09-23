// ************************************************
// Log to SD card
// ************************************************

void LogToSD() {
  unsigned long sd_current_mmillis = millis();
  DateTime now = rtc.now();
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.println(F("LOGGING TO SD"));

  char* time_format = "YYYY/MM/DD/hh/YYMMDDhh.txt";
  char* lines_format = "YYYY/MM/DD/hh/hh.txt";
  char* formatted_file_name = now.toString(time_format);
  char* formatted_line_file_name = now.toString(lines_format);
  char* directory_format = "YYYY/MM/DD/hh";
  char* formatted_directory_name = now.toString(directory_format);
  Serial.println(formatted_directory_name);
  Serial.println(formatted_file_name);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  SD.mkdir(formatted_directory_name);

  // update accompanying line-count file
  File line_file = SD.open(formatted_line_file_name, FILE_WRITE);
  int line_file_size = line_file.size();
  if (!line_file_size) {
    line_file.println("1");
    line_file.close();
  } else {
    char lines_buffer[10];
    line_file.seek(0);
    byte read;
    for (int i = 0; i < 10 && read != -1; i++) {
      read = line_file.read();
      lines_buffer[i] = (char)read;
    }
    int line_count = atoi(lines_buffer);
    line_file.close();
    SD.remove(formatted_line_file_name);
    line_file = SD.open(formatted_line_file_name, FILE_WRITE);
    char line_count_buffer[10];
    sprintf(line_count_buffer, "%d", line_count + 1);
    line_file.println(line_count_buffer);
    line_file.close();
  }

  my_file = SD.open(formatted_file_name, FILE_WRITE);
  // create information string with timestamp
  char formatted_sd_string[RECORD_LENGTH];
  memset(formatted_sd_string, 0, RECORD_LENGTH);
  char time_buffer[100];
  memset(time_buffer, 0, 100);
  char var_buffer[100];
  memset(var_buffer, 0, 100);
  DateTime temp_now = rtc.now();
  FormatDouble(temp_now.year(), 0, time_buffer, 0xffff);
  strcat(formatted_sd_string, time_buffer);
  strcat(formatted_sd_string, "/");
  FormatDouble(temp_now.month(), 0, time_buffer, 0xffff);
  strcat(formatted_sd_string, time_buffer);
  strcat(formatted_sd_string, "/");
  FormatDouble(temp_now.day(), 0, time_buffer, 0xffff);
  strcat(formatted_sd_string, time_buffer);
  strcat(formatted_sd_string, " ");
  FormatDouble(temp_now.hour(), 0, time_buffer, 0xffff);
  strcat(formatted_sd_string, time_buffer);
  strcat(formatted_sd_string, ":");
  if (temp_now.minute() < 10) {
    strcat(formatted_sd_string, "0");
  }
  FormatDouble(temp_now.minute(), 0, time_buffer, 0xffff);
  strcat(formatted_sd_string, time_buffer);
  strcat(formatted_sd_string, ":");
  if (temp_now.second() < 10) {
    strcat(formatted_sd_string, "0");
  }
  FormatDouble(temp_now.second(), 0, time_buffer, 0xffff);
  strcat(formatted_sd_string, time_buffer);

  snprintf(var_buffer, 100, "%d", tank_id);
  strcat(formatted_sd_string, ",");
  strcat(formatted_sd_string, var_buffer);
  FormatDouble(temp, 2, var_buffer, 0xffff);
  strcat(formatted_sd_string, ",");
  strcat(formatted_sd_string, var_buffer);
  FormatDouble(temp_set, 2, var_buffer, 0xffff);
  strcat(formatted_sd_string, ",");
  strcat(formatted_sd_string, var_buffer);
  FormatDouble(pH, 3, var_buffer, 0xffff);
  strcat(formatted_sd_string, ",");
  strcat(formatted_sd_string, var_buffer);
  FormatDouble(ph_set, 3, var_buffer, 0xffff);
  strcat(formatted_sd_string, ",");
  strcat(formatted_sd_string, var_buffer);
  FormatDouble(on_time, 0, var_buffer, 0xffff);
  strcat(formatted_sd_string, ",");
  strcat(formatted_sd_string, var_buffer);

  Serial.print(F("formatted_sd_string SIZE: "));
  Serial.println(strlen(formatted_sd_string));
  Serial.print(F("ADDING: "));
  Serial.println(RECORD_LENGTH - strlen(formatted_sd_string));
  for (strlen(formatted_sd_string); strlen(formatted_sd_string) < RECORD_LENGTH;) {
    strcat(formatted_sd_string, " ");
  }
  Serial.print(F("AFTER EDIT formatted_sd_string SIZE: "));
  Serial.println(strlen(formatted_sd_string));
  my_file.println(formatted_sd_string);
  my_file.close();
  Serial.println(formatted_sd_string);
}
